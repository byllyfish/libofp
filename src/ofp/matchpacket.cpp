// Copyright (c) 2015-2016 William W. Fisher (at gmail dot com)
// This file is distributed under the MIT License.

#include "ofp/matchpacket.h"
#include "ofp/constants.h"
#include "ofp/macaddress.h"
#include "ofp/oxmfields.h"
#include "ofp/pkt.h"

using namespace ofp;

// OpenFlow specifies the alignment of the packet data. It is aligned so
// an IP, IPv6, ARP header portion is aligned to an 8-byte boundary.
// All decode functions in this class assume this alignment. However,
// if the input data is not aligned, we fix it up so it is -- this involves
// making a copy.

static bool isAlignedPacket(const UInt8 *ptr) {
  return (reinterpret_cast<uintptr_t>(ptr) & 0x07) == 2;
}

MatchPacket::MatchPacket(const ByteRange &data, bool warnMisaligned) {
  if (isAlignedPacket(data.data())) {
    decode(data.data(), data.size());
    return;
  }

  auto misalignment = reinterpret_cast<uintptr_t>(data.data()) & 0x07;
  assert(misalignment != 2 && misalignment < 8);

  if (warnMisaligned) {
    log_warning("MatchPacket: Misaligned input: ", misalignment);
  }

  // Make a copy of the packet that is appropriately aligned.
  size_t pad = (10 - misalignment) % 8;
  ByteList alignedData;
  alignedData.addZeros(pad);
  alignedData.add(data.data(), data.size());
  decode(alignedData.data() + pad, alignedData.size() - pad);
}

// Flag used internally to represent OFPIEH_DEST when it appears in an IPv6
// header chain just before a Router header.

const UInt32 OFPIEH_DEST_ROUTER = 1UL << 31;

void MatchPacket::decode(const UInt8 *pkt, size_t length) {
  assert(isAlignedPacket(pkt));

  decodeEthernet(pkt, length);

  // If all the data is not accounted for, report offset using X_PKT_MARK.
  if (offset_ < length) {
    match_.add(X_PKT_MARK{UInt16_narrow_cast(offset_)});
  }
}

void MatchPacket::decodeEthernet(const UInt8 *pkt, size_t length) {
  assert(isAlignedPacket(pkt));

  auto eth = pkt::Ethernet::cast(pkt, length);
  if (!eth) {
    return;
  }

  match_.add(OFB_ETH_DST(eth->dst));
  match_.add(OFB_ETH_SRC(eth->src));
  match_.add(OFB_ETH_TYPE(eth->type));

  pkt += sizeof(pkt::Ethernet);
  length -= sizeof(pkt::Ethernet);
  offset_ += sizeof(pkt::Ethernet);

  switch (eth->type) {
    case DATALINK_ARP:
      decodeARP(pkt, length);
      break;

    case DATALINK_IPV4:
      decodeIPv4(pkt, length);
      break;

    case DATALINK_IPV6:
      decodeIPv6(pkt, length);
      break;

    case DATALINK_LLDP:
      decodeLLDP(pkt, length);
      break;

    default:
      log_warning("MatchPacket: Unknown ethernet type", eth->type);
      break;
  }
}

void MatchPacket::decodeARP(const UInt8 *pkt, size_t length) {
  auto arp = pkt::Arp::cast(pkt, length);
  if (!arp) {
    return;
  }

  if (std::memcmp(arp->prefix, "\x00\x01\x08\x00\x06\x04",
                  sizeof(arp->prefix)) != 0) {
    log_warning("MatchPacket: Unexpected arp prefix", log::hex(arp->prefix));
    return;
  }

  match_.add(OFB_ARP_OP(arp->op));
  match_.add(OFB_ARP_SPA(arp->spa));
  match_.add(OFB_ARP_TPA(arp->tpa));
  match_.add(OFB_ARP_SHA(arp->sha));
  match_.add(OFB_ARP_THA(arp->tha));

  offset_ += sizeof(pkt::Arp);

  if (length > sizeof(pkt::Arp)) {
    log_warning("MatchPacket: Ignoring extra data in arp message");
  }
}

void MatchPacket::decodeIPv4(const UInt8 *pkt, size_t length) {
  auto ip = pkt::IPv4Hdr::cast(pkt, length);
  if (!ip) {
    return;
  }

  UInt8 vers = ip->ver >> 4;
  UInt8 ihl = ip->ver & 0x0F;

  if (vers != pkt::kIPv4Version) {
    log_warning("MatchPacket: Unexpected IPv4 version", vers);
    return;
  }

  if (ihl < 5) {
    log_warning("MatchPacket: Unexpected IPv4 header length", ihl);
    return;
  }

  UInt8 dscp = ip->tos >> 2;
  UInt8 ecn = ip->tos & 0x03;

  match_.add(OFB_IP_DSCP{dscp});
  match_.add(OFB_IP_ECN{ecn});
  match_.add(OFB_IP_PROTO{ip->proto});
  match_.add(OFB_IPV4_SRC{ip->src});
  match_.add(OFB_IPV4_DST{ip->dst});

  if (ip->frag) {
    match_.add(NXM_NX_IP_FRAG{pkt::nxmFragmentType(ip->frag)});
  }

  match_.add(NXM_NX_IP_TTL{ip->ttl});

  assert(ihl <= 15);

  unsigned hdrLen = ihl * 4;
  if (hdrLen > length) {
    log_warning("MatchPacket: Extended IPv4 Header too long", hdrLen);
    return;
  }

  pkt += hdrLen;
  length -= hdrLen;
  offset_ += hdrLen;

  decodeIPv4_NextHdr(pkt, length, ip->proto);
}

void MatchPacket::decodeIPv4_NextHdr(const UInt8 *pkt, size_t length,
                                     UInt8 nextHdr) {
  switch (nextHdr) {
    case PROTOCOL_ICMP:
      decodeICMPv4(pkt, length);
      break;

    case PROTOCOL_TCP:
      decodeTCP(pkt, length);
      break;

    case PROTOCOL_UDP:
      decodeUDP(pkt, length);
      break;

    default:
      log_warning("MatchPacket: Unknown IPv4 protocol", nextHdr);
      break;
  }
}

void MatchPacket::decodeIPv6(const UInt8 *pkt, size_t length) {
  auto ip = pkt::IPv6Hdr::cast(pkt, length);
  if (!ip) {
    return;
  }

  UInt32 verClassLabel = ip->verClassLabel;
  UInt8 vers = verClassLabel >> 28;
  if (vers != pkt::kIPv6Version) {
    log_warning("MatchPacket: Unexpected IPv6 version", vers);
    return;
  }

  UInt8 trafCls = (verClassLabel >> 20) & 0x0FF;
  UInt32 flowLabel = verClassLabel & 0x000FFFFF;

  UInt8 dscp = trafCls >> 2;
  UInt8 ecn = trafCls & 0x03;

  match_.add(OFB_IP_DSCP{dscp});
  match_.add(OFB_IP_ECN{ecn});
  match_.add(OFB_IPV6_SRC{ip->src});
  match_.add(OFB_IPV6_DST{ip->dst});
  match_.add(OFB_IPV6_FLABEL{flowLabel});

  pkt += sizeof(pkt::IPv6Hdr);
  length -= sizeof(pkt::IPv6Hdr);
  offset_ += sizeof(pkt::IPv6Hdr);

  decodeIPv6_NextHdr(pkt, length, ip->nextHeader);
}

void MatchPacket::decodeTCP(const UInt8 *pkt, size_t length) {
  auto tcp = pkt::TCPHdr::cast(pkt, length);
  if (!tcp) {
    return;
  }

  UInt16 flags = tcp->flags & 0x0FFF;
  match_.add(OFB_TCP_SRC{tcp->srcPort});
  match_.add(OFB_TCP_DST{tcp->dstPort});
  match_.add(NXM_NX_TCP_FLAGS{flags});

  offset_ += sizeof(pkt::TCPHdr);
}

void MatchPacket::decodeUDP(const UInt8 *pkt, size_t length) {
  auto udp = pkt::UDPHdr::cast(pkt, length);
  if (!udp) {
    return;
  }

  match_.add(OFB_UDP_SRC{udp->srcPort});
  match_.add(OFB_UDP_DST{udp->dstPort});

  offset_ += sizeof(pkt::UDPHdr);
}

void MatchPacket::decodeIPv6_NextHdr(const UInt8 *pkt, size_t length,
                                     UInt8 nextHdr) {
  UInt32 hdrFlags = 0;

  while (nextHdr != pkt::IPv6Ext_NoNextHeader) {
    switch (nextHdr) {
      case PROTOCOL_TCP:
        match_.addOrderedUnchecked(OFB_IP_PROTO{nextHdr});
        decodeTCP(pkt, length);
        nextHdr = pkt::IPv6Ext_NoNextHeader;
        break;

      case PROTOCOL_UDP:
        match_.addOrderedUnchecked(OFB_IP_PROTO{nextHdr});
        decodeUDP(pkt, length);
        nextHdr = pkt::IPv6Ext_NoNextHeader;
        break;

      case PROTOCOL_ICMPV6:
        match_.addOrderedUnchecked(OFB_IP_PROTO{nextHdr});
        decodeICMPv6(pkt, length);
        nextHdr = pkt::IPv6Ext_NoNextHeader;
        break;

      default:
        // Record that we saw this extension header, then advance to
        // the next.
        nextHdr = nextIPv6ExtHdr(nextHdr, pkt, length, hdrFlags);
        break;
    }
  }

  if (hdrFlags == 0) {
    hdrFlags |= OFPIEH_NONEXT;
  } else if (hdrFlags & OFPIEH_DEST_ROUTER) {
    hdrFlags = (hdrFlags & 0x0ffff) | OFPIEH_DEST;
  }

  match_.add(OFB_IPV6_EXTHDR{static_cast<OFPIPv6ExtHdrFlags>(hdrFlags)});
}

void MatchPacket::decodeICMPv4(const UInt8 *pkt, size_t length) {
  auto icmp = pkt::ICMPHdr::cast(pkt, length);
  if (!icmp) {
    return;
  }

  match_.add(OFB_ICMPV4_TYPE{icmp->type});
  match_.add(OFB_ICMPV4_CODE{icmp->code});

  offset_ += sizeof(pkt::ICMPHdr);
}

void MatchPacket::decodeICMPv6(const UInt8 *pkt, size_t length) {
  auto icmp = pkt::ICMPHdr::cast(pkt, length);
  if (!icmp) {
    return;
  }

  match_.add(OFB_ICMPV6_TYPE{icmp->type});
  match_.add(OFB_ICMPV6_CODE{icmp->code});

  offset_ += sizeof(pkt::ICMPHdr);

  // TODO(bfish): neighbor discovery msgs.
}

void MatchPacket::decodeLLDP(const UInt8 *pkt, size_t length) {
  // LLDP packets are composed of TLV sections (tag-length-value).

  while (length > 0) {
    auto lldp = pkt::LLDPTlv::cast(pkt, length);
    if (!lldp) {
      log_warning("decodeLLDP: missing lldp end tlv");
      return;
    }

    auto jumpSize = lldp->length() + 2;
    if (jumpSize > length) {
      log_warning("decodeLLDP: malformed lldp tlv");
      return;  // ignore the rest
    }

    switch (lldp->type()) {
      case pkt::LLDPTlv::END:
        offset_ += jumpSize;
        return;  // all done; ignore anything else

      case pkt::LLDPTlv::CHASSIS_ID:
        match_.add(X_LLDP_CHASSIS_ID{lldp->value()});
        break;

      case pkt::LLDPTlv::PORT_ID:
        match_.add(X_LLDP_PORT_ID{lldp->value()});
        break;

      case pkt::LLDPTlv::TTL:
        match_.add(X_LLDP_TTL{lldp->value16()});
        break;
    }

    assert(length >= jumpSize);

    pkt += jumpSize;
    length -= jumpSize;
    offset_ += jumpSize;
  }
}

UInt8 MatchPacket::nextIPv6ExtHdr(UInt8 currHdr, const UInt8 *&pkt,
                                  size_t &length, UInt32 &flags) {
  // RFC 2460 Section 4.1: Extension Header Order
  //
  //       Hop-by-Hop Options header
  //       Destination Options header (*)
  //       Routing header
  //       Fragment header
  //       Authentication header
  //       Encapsulating Security Payload header
  //       Destination Options header (*)
  //
  // Each extension header should occur at most once, except for the
  // Destination Options header which should occur at most twice (once
  // before a Routing header and once before the upper-layer header).

  assert(IsPtrAligned(pkt, 8));
  assert(currHdr != pkt::IPv6Ext_NoNextHeader);

  auto ext = pkt::IPv6ExtHdr::cast(pkt, length);
  if (!ext) {
    return pkt::IPv6Ext_NoNextHeader;
  }

  // Masks for ordering extension headers by examining predecessors.
  const UInt32 kPreHopMask = 0;
  const UInt32 kPreDestRouterMask = kPreHopMask | OFPIEH_HOP;
  const UInt32 kPreRouterMask = kPreDestRouterMask | OFPIEH_DEST_ROUTER;
  const UInt32 kPreFragmentMask = kPreRouterMask | OFPIEH_ROUTER;
  const UInt32 kPreAuthMask = kPreFragmentMask | OFPIEH_FRAG;
  const UInt32 kPreESPMask = kPreAuthMask | OFPIEH_AUTH;
  const UInt32 kPreDestMask = kPreESPMask | OFPIEH_ESP;

  UInt8 nextHdr = ext->nextHeader;

  switch (currHdr) {
    case pkt::IPv6Ext_Hop:
      countIPv6ExtHdr(flags, OFPIEH_HOP, kPreHopMask);
      break;

    case pkt::IPv6Ext_Dest:
      if (nextHdr == pkt::IPv6Ext_Router) {
        countIPv6ExtHdr(flags, OFPIEH_DEST_ROUTER, kPreDestRouterMask);
      } else if (!pkt::IPv6NextHeaderIsExtension(nextHdr)) {
        countIPv6ExtHdr(flags, OFPIEH_DEST, kPreDestMask);
      } else {
        if (flags & OFPIEH_DEST)
          flags |= OFPIEH_UNREP;
        flags |= (OFPIEH_DEST | OFPIEH_UNSEQ);
      }
      break;

    case pkt::IPv6Ext_Router:
      countIPv6ExtHdr(flags, OFPIEH_ROUTER, kPreRouterMask);
      break;

    case pkt::IPv6Ext_Fragment:
      countIPv6ExtHdr(flags, OFPIEH_FRAG, kPreFragmentMask);
      break;

    case pkt::IPv6Ext_Auth:
      countIPv6ExtHdr(flags, OFPIEH_AUTH, kPreAuthMask);
      break;

    case pkt::IPv6Ext_ESP:
      countIPv6ExtHdr(flags, OFPIEH_ESP, kPreESPMask);
      break;

    case pkt::IPv6Ext_Mobility:
    case pkt::IPv6Ext_HostIdentity:
    case pkt::IPv6Ext_Shim6:
    case pkt::IPv6Ext_Experimental253:
    case pkt::IPv6Ext_Experimental254:
      // OpenFlow protocol ignores these extension headers.
      break;

    default:
      log_warning("MatchPacket: Unrecognized IPv6 nextHeader", currHdr);
      return pkt::IPv6Ext_NoNextHeader;
  }

  // Fragment header is always 8 bytes in size. Otherwise, we need to compute
  // header size from `hdrLen`.
  size_t extHdrLen = 8;
  if (currHdr != pkt::IPv6Ext_Fragment) {
    extHdrLen = (ext->hdrLen + 1) * 8;
  }

  if (extHdrLen > length) {
    log_warning("MatchPacket: IPv6 ext header extends past end of pkt");
    return pkt::IPv6Ext_NoNextHeader;
  }

  pkt += extHdrLen;
  length -= extHdrLen;
  offset_ += extHdrLen;

  return nextHdr;
}

void MatchPacket::countIPv6ExtHdr(UInt32 &flags, UInt32 hdr,
                                  UInt32 precedingHdrs) {
  if (flags & ~(hdr | precedingHdrs)) {
    flags |= OFPIEH_UNSEQ;
  }
  if (flags & hdr) {
    flags |= OFPIEH_UNREP;
  }
  flags |= hdr;
}

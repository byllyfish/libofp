// Copyright (c) 2015-2017 William W. Fisher (at gmail dot com)
// This file is distributed under the MIT License.

#include "ofp/ipv6address.h"
#include <arpa/inet.h>
#include "ofp/byteorder.h"
#include "ofp/log.h"

using namespace ofp;

IPv6Address::IPv6Address(const IPv4Address &addr) {
  std::memcpy(&addr_[12], &addr, 4);
  addr_[10] = 0xFF;
  addr_[11] = 0xFF;
  std::memset(addr_.data(), 0, 10);
}

IPv6Address::IPv6Address(const std::string &s) {
  if (!parse(s))
    clear();
}

UInt32 IPv6Address::zone() const {
  if (!isLinkLocal())
    return 0;
  const Big16 lo = Big16_unaligned(&addr_[2]);
  const Big16 hi = Big16_unaligned(&addr_[4]);
  UInt32 x = hi;
  return (x << 16) | lo;
}

void IPv6Address::setZone(UInt32 zone) {
  assert(isLinkLocal());

  Big16 lo = UInt16_narrow_cast(zone);
  Big16 hi = UInt16_narrow_cast(zone >> 16);
  std::memcpy(&addr_[2], &lo, 2);
  std::memcpy(&addr_[4], &hi, 2);
}

bool IPv6Address::parse(const std::string &s, bool parseIPv4) {
  // Check for %zone option.
  auto pct = s.find('%');
  if (pct != std::string::npos) {
    // If it's not an IPv6 address, the parse fails.
    if (!parseIPv6Address(s.substr(0, pct)))
      return false;

    // If IPv6 address is not link-local, the parse fails.
    if (!isLinkLocal())
      return false;

    // If zone is not an integer, the parse fails.
    auto zoneStr = s.substr(pct + 1);
    if (zoneStr.empty())
      return false;

    UInt32 zone;
    if (llvm::StringRef{zoneStr}.getAsInteger(10, zone))
      return false;

    setZone(zone);

    return true;
  }

  assert(pct == std::string::npos);

  if (parseIPv6Address(s))
    return true;

  if (!parseIPv4)
    return false;

  return parseIPv4Address(s);
}

bool IPv6Address::parseIPv6Address(const std::string &s) {
  int result = inet_pton(AF_INET6, s.c_str(), addr_.data());
  return result > 0;
}

bool IPv6Address::parseIPv4Address(const std::string &s) {
  IPv4Address addr;
  if (addr.parse(s)) {
    *this = IPv6Address(addr);
    return true;
  }
  return false;
}

/// Output IPv6Address to stream in V6 format.
void IPv6Address::outputV6(llvm::raw_ostream &os) const {
  IPv6Address temp{*this};
  UInt32 my_zone = zone();
  if (my_zone) {
    temp.setZone(0);
  }

  char buf[INET6_ADDRSTRLEN];
  const UInt8 *data = temp.toArray().data();
  const char *result = inet_ntop(AF_INET6, data, buf, sizeof(buf));

  os << (result ? result : "<inet_ntop_error6>");
  if (my_zone) {
    os << '%' << my_zone;
  }
}

namespace ofp {

llvm::raw_ostream &operator<<(llvm::raw_ostream &os, const IPv6Address &value) {
  // Output mapped IPv4 address as dotted quad.
  if (value.isV4Mapped()) {
    return os << value.toV4();
  }

  value.outputV6(os);
  return os;
}

}  // namespace ofp

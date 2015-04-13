// Copyright 2014-present Bill Fisher. All rights reserved.

#ifndef OFP_YAML_YPACKETOUT_H_
#define OFP_YAML_YPACKETOUT_H_

#include "ofp/packetout.h"

namespace llvm {
namespace yaml {

const char *const kPacketOutSchema = R"""({Message/PACKET_OUT}
type: 'PACKET_OUT'
msg:
  buffer_id: BufferID
  in_port: PortNumber
  actions: [{Action}...]
  data: HexString
  _data_pkt: [{Field}...]         # Output only; Optional
)""";

template <>
struct MappingTraits<ofp::PacketOut> {
  static void mapping(IO &io, ofp::PacketOut &msg) {
    using namespace ofp;

    BufferID bufferId = msg.bufferId();
    PortNumber inPort = msg.inPort();

    io.mapRequired("buffer_id", bufferId);
    io.mapRequired("in_port", inPort);

    ActionRange actions = msg.actions();
    io.mapRequired("actions", actions);

    ByteRange enetFrame = msg.enetFrame();
    io.mapRequired("data", enetFrame);

    if (ofp::yaml::GetIncludePktMatchFromContext(io)) {
      ofp::MatchPacket mp{enetFrame, false};
      io.mapRequired("_data_pkt", mp);
    }
  }
};

template <>
struct MappingTraits<ofp::PacketOutBuilder> {
  static void mapping(IO &io, ofp::PacketOutBuilder &msg) {
    using namespace ofp;

    BufferID bufferId;
    io.mapRequired("buffer_id", bufferId);
    msg.setBufferId(bufferId);

    io.mapRequired("in_port", msg.msg_.inPort_);

    io.mapRequired("actions", msg.actions_);
    io.mapRequired("data", msg.enetFrame_);
  }
};

}  // namespace yaml
}  // namespace llvm

#endif  // OFP_YAML_YPACKETOUT_H_

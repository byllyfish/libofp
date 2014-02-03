#ifndef OFP_YAML_YAGGREGATESTATSREPLY_H
#define OFP_YAML_YAGGREGATESTATSREPLY_H

#include "ofp/mpaggregatestatsreply.h"

namespace llvm { // <namespace llvm>
namespace yaml { // <namespace yaml>

template <>
struct MappingTraits<ofp::MPAggregateStatsReply> {

  static void mapping(IO &io, ofp::MPAggregateStatsReply &msg) {
    io.mapRequired("packet_count", msg.packetCount_);
    io.mapRequired("byte_count", msg.byteCount_);
    io.mapRequired("flow_count", msg.flowCount_);
  }
};

template <>
struct MappingTraits<ofp::MPAggregateStatsReplyBuilder> {

  static void mapping(IO &io, ofp::MPAggregateStatsReplyBuilder &msg) {
    io.mapRequired("packet_count", msg.msg_.packetCount_);
    io.mapRequired("byte_count", msg.msg_.byteCount_);
    io.mapRequired("flow_count", msg.msg_.flowCount_);
  }
};

} // </namespace yaml>
} // </namespace llvm>

#endif // OFP_YAML_YAGGREGATESTATSREPLY_H

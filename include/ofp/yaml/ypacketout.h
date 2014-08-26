//  ===== ---- ofp/yaml/ypacketout.h -----------------------*- C++ -*- =====  //
//
//  Copyright (c) 2013 William W. Fisher
//
//  Licensed under the Apache License, Version 2.0 (the "License");
//  you may not use this file except in compliance with the License.
//  You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
//  Unless required by applicable law or agreed to in writing, software
//  distributed under the License is distributed on an "AS IS" BASIS,
//  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//  See the License for the specific language governing permissions and
//  limitations under the License.
//
//  ===== ------------------------------------------------------------ =====  //
/// \file
/// \brief Defines the llvm::yaml::MappingTraits for the PacketOut and
/// PacketOutBuilder classes.
//  ===== ------------------------------------------------------------ =====  //

#ifndef OFP_YAML_YPACKETOUT_H_
#define OFP_YAML_YPACKETOUT_H_

#include "ofp/packetout.h"

namespace llvm {
namespace yaml {

//---
// type: OFPT_PACKET_OUT
// msg:
//   buffer_id: <UInt32>      { Required }
//   in_port: <UInt32>        { Required }
//   actions: [ <Action> ]    { Required }
//   data: <Bytes>            { Required }
//...

template <>
struct MappingTraits<ofp::PacketOut> {

  static void mapping(IO &io, ofp::PacketOut &msg) {
    using namespace ofp;

    Hex32 bufferID = msg.bufferId();
    Hex32 inPort = msg.inPort();

    io.mapRequired("buffer_id", bufferID);
    io.mapRequired("in_port", inPort);

    ActionRange actions = msg.actions();
    io.mapRequired("actions", actions);

    ByteRange enetFrame = msg.enetFrame();
    io.mapRequired("data", enetFrame);
  }
};

template <>
struct MappingTraits<ofp::PacketOutBuilder> {

  static void mapping(IO &io, ofp::PacketOutBuilder &msg) {
    using namespace ofp;

    io.mapRequired("buffer_id", msg.msg_.bufferId_);
    io.mapRequired("in_port", msg.msg_.inPort_);

    io.mapRequired("actions", msg.actions_);
    io.mapRequired("data", msg.enetFrame_);
  }
};

}  // namespace yaml
}  // namespace llvm

#endif  // OFP_YAML_YPACKETOUT_H_

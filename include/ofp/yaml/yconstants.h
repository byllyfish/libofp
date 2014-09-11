//  ===== ---- ofp/yaml/yconstants.h -----------------------*- C++ -*- =====  //
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
/// \brief Defines the llvm::yaml::ScalarEnumerationTraits for enumerated
/// constants:
///
///   - OFPType
///   - OFPMultipartType
///   - OFPInstructionType
///   - OFPActionType
///   - OFPPacketInReason
///   - OFPMeterBandType
///
//  ===== ------------------------------------------------------------ =====  //

#ifndef OFP_YAML_YCONSTANTS_H_
#define OFP_YAML_YCONSTANTS_H_

#include "ofp/constants.h"
#include "ofp/yaml/enumconverter.h"

namespace llvm {
namespace yaml {

#define OFP_YAML_ENUMCASE(val) io.enumCase(value, #val, ofp::val)

template <>
struct ScalarEnumerationTraits<ofp::OFPType> {
  static void enumeration(IO &io, ofp::OFPType &value) {
    OFP_YAML_ENUMCASE(OFPT_HELLO);
    OFP_YAML_ENUMCASE(OFPT_ERROR);
    OFP_YAML_ENUMCASE(OFPT_ECHO_REQUEST);
    OFP_YAML_ENUMCASE(OFPT_ECHO_REPLY);
    OFP_YAML_ENUMCASE(OFPT_EXPERIMENTER);
    OFP_YAML_ENUMCASE(OFPT_FEATURES_REQUEST);
    OFP_YAML_ENUMCASE(OFPT_FEATURES_REPLY);
    OFP_YAML_ENUMCASE(OFPT_GET_CONFIG_REQUEST);
    OFP_YAML_ENUMCASE(OFPT_GET_CONFIG_REPLY);
    OFP_YAML_ENUMCASE(OFPT_SET_CONFIG);
    OFP_YAML_ENUMCASE(OFPT_PACKET_IN);
    OFP_YAML_ENUMCASE(OFPT_FLOW_REMOVED);
    OFP_YAML_ENUMCASE(OFPT_PORT_STATUS);
    OFP_YAML_ENUMCASE(OFPT_PACKET_OUT);
    OFP_YAML_ENUMCASE(OFPT_FLOW_MOD);
    OFP_YAML_ENUMCASE(OFPT_GROUP_MOD);
    OFP_YAML_ENUMCASE(OFPT_PORT_MOD);
    OFP_YAML_ENUMCASE(OFPT_TABLE_MOD);
    OFP_YAML_ENUMCASE(OFPT_MULTIPART_REQUEST);
    OFP_YAML_ENUMCASE(OFPT_MULTIPART_REPLY);
    OFP_YAML_ENUMCASE(OFPT_BARRIER_REQUEST);
    OFP_YAML_ENUMCASE(OFPT_BARRIER_REPLY);
    OFP_YAML_ENUMCASE(OFPT_QUEUE_GET_CONFIG_REQUEST);
    OFP_YAML_ENUMCASE(OFPT_QUEUE_GET_CONFIG_REPLY);
    OFP_YAML_ENUMCASE(OFPT_ROLE_REQUEST);
    OFP_YAML_ENUMCASE(OFPT_ROLE_REPLY);
    OFP_YAML_ENUMCASE(OFPT_GET_ASYNC_REQUEST);
    OFP_YAML_ENUMCASE(OFPT_GET_ASYNC_REPLY);
    OFP_YAML_ENUMCASE(OFPT_SET_ASYNC);
    OFP_YAML_ENUMCASE(OFPT_METER_MOD);
  }
};

template <>
struct ScalarEnumerationTraits<ofp::OFPMultipartType> {
  static void enumeration(IO &io, ofp::OFPMultipartType &value) {
    OFP_YAML_ENUMCASE(OFPMP_DESC);
    OFP_YAML_ENUMCASE(OFPMP_FLOW);
    OFP_YAML_ENUMCASE(OFPMP_AGGREGATE);
    OFP_YAML_ENUMCASE(OFPMP_TABLE);
    OFP_YAML_ENUMCASE(OFPMP_PORT_STATS);
    OFP_YAML_ENUMCASE(OFPMP_QUEUE);
    OFP_YAML_ENUMCASE(OFPMP_GROUP);
    OFP_YAML_ENUMCASE(OFPMP_GROUP_DESC);
    OFP_YAML_ENUMCASE(OFPMP_GROUP_FEATURES);
    OFP_YAML_ENUMCASE(OFPMP_METER);
    OFP_YAML_ENUMCASE(OFPMP_METER_CONFIG);
    OFP_YAML_ENUMCASE(OFPMP_METER_FEATURES);
    OFP_YAML_ENUMCASE(OFPMP_TABLE_FEATURES);
    OFP_YAML_ENUMCASE(OFPMP_PORT_DESC);
    OFP_YAML_ENUMCASE(OFPMP_EXPERIMENTER);
  }
};

template <>
struct ScalarEnumerationTraits<ofp::OFPInstructionType> {
  static void enumeration(IO &io, ofp::OFPInstructionType &value) {
    OFP_YAML_ENUMCASE(OFPIT_GOTO_TABLE);
    OFP_YAML_ENUMCASE(OFPIT_WRITE_METADATA);
    OFP_YAML_ENUMCASE(OFPIT_WRITE_ACTIONS);
    OFP_YAML_ENUMCASE(OFPIT_APPLY_ACTIONS);
    OFP_YAML_ENUMCASE(OFPIT_CLEAR_ACTIONS);
    OFP_YAML_ENUMCASE(OFPIT_METER);
    OFP_YAML_ENUMCASE(OFPIT_EXPERIMENTER);
  }
};


template <>
struct ScalarTraits<ofp::OFPPacketInReason> {
  static ofp::yaml::EnumConverter<ofp::OFPPacketInReason> converter;

  static void output(const ofp::OFPPacketInReason &value, void *ctxt,
                     llvm::raw_ostream &out) {
    llvm::StringRef scalar;
    if (converter.convert(value, &scalar)) {
      out << scalar;
    } else {
      out << format("0x%02X", value);
    }
  }

  static StringRef input(StringRef scalar, void *ctxt,
                         ofp::OFPPacketInReason &value) {
    if (converter.convert(scalar, &value)) {
      return "";
    }

    return "Invalid enumerated constant.";
  }
};


template <>
struct ScalarTraits<ofp::OFPFlowModCommand> {
  static ofp::yaml::EnumConverter<ofp::OFPFlowModCommand> converter;

  static void output(const ofp::OFPFlowModCommand &value, void *ctxt,
                     llvm::raw_ostream &out) {
    llvm::StringRef scalar;
    if (converter.convert(value, &scalar)) {
      out << scalar;
    } else {
      out << format("0x%02X", value);
    }
  }

  static StringRef input(StringRef scalar, void *ctxt,
                         ofp::OFPFlowModCommand &value) {
    if (converter.convert(scalar, &value)) {
      return "";
    }

    return "Invalid enumerated constant.";
  }
};

template <>
struct ScalarEnumerationTraits<ofp::OFPMeterBandType> {
  static void enumeration(IO &io, ofp::OFPMeterBandType &value) {
    OFP_YAML_ENUMCASE(OFPMBT_DROP);
    OFP_YAML_ENUMCASE(OFPMBT_DSCP_REMARK);
    OFP_YAML_ENUMCASE(OFPMBT_EXPERIMENTER);
  }
};

#undef OFP_YAML_ENUMCASE

}  // namespace yaml
}  // namespace llvm

#endif  // OFP_YAML_YCONSTANTS_H_

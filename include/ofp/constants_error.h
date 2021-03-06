// Copyright (c) 2015-2018 William W. Fisher (at gmail dot com)
// This file is distributed under the MIT License.

#ifndef OFP_CONSTANTS_ERROR_H_
#define OFP_CONSTANTS_ERROR_H_

#include "ofp/constants.h"

namespace ofp {

enum OFPErrorType : UInt16 {
  OFPET_HELLO_FAILED = 0,
  OFPET_BAD_REQUEST = 1,
  OFPET_BAD_ACTION = 2,
  OFPET_BAD_INSTRUCTION = 3,
  OFPET_BAD_MATCH = 4,
  OFPET_FLOW_MOD_FAILED = 5,
  OFPET_GROUP_MOD_FAILED = 6,
  OFPET_PORT_MOD_FAILED = 7,
  OFPET_TABLE_MOD_FAILED = 8,
  OFPET_QUEUE_OP_FAILED = 9,
  OFPET_SWITCH_CONFIG_FAILED = 10,
  OFPET_ROLE_REQUEST_FAILED = 11,
  OFPET_METER_MOD_FAILED = 12,
  OFPET_TABLE_FEATURES_FAILED = 13,
  OFPET_BAD_PROPERTY = 14,
  OFPET_ASYNC_CONFIG_FAILED = 15,
  OFPET_FLOW_MONITOR_FAILED = 16,
  OFPET_BUNDLE_FAILED = 17,
  OFPET_EXPERIMENTER = 0xffff,

  OFPET_UNKNOWN_FLAG = 0x8000
};

constexpr UInt32 OFPMakeErrorCode(UInt16 type, UInt16 code) {
  return (static_cast<UInt32>(type) << 16) | code;
}

enum OFPErrorCode : UInt32 {
  OFPHFC_INCOMPATIBLE = OFPMakeErrorCode(OFPET_HELLO_FAILED, 0),
  OFPHFC_EPERM = OFPMakeErrorCode(OFPET_HELLO_FAILED, 1),

  OFPBRC_BAD_VERSION = OFPMakeErrorCode(OFPET_BAD_REQUEST, 0),
  OFPBRC_BAD_TYPE = OFPMakeErrorCode(OFPET_BAD_REQUEST, 1),
  OFPBRC_BAD_MULTIPART = OFPMakeErrorCode(OFPET_BAD_REQUEST, 2),
  OFPBRC_BAD_EXPERIMENTER = OFPMakeErrorCode(OFPET_BAD_REQUEST, 3),
  OFPBRC_BAD_EXP_TYPE = OFPMakeErrorCode(OFPET_BAD_REQUEST, 4),
  OFPBRC_EPERM = OFPMakeErrorCode(OFPET_BAD_REQUEST, 5),
  OFPBRC_BAD_LEN = OFPMakeErrorCode(OFPET_BAD_REQUEST, 6),
  OFPBRC_BUFFER_EMPTY = OFPMakeErrorCode(OFPET_BAD_REQUEST, 7),
  OFPBRC_BUFFER_UNKNOWN = OFPMakeErrorCode(OFPET_BAD_REQUEST, 8),
  OFPBRC_BAD_TABLE_ID = OFPMakeErrorCode(OFPET_BAD_REQUEST, 9),
  OFPBRC_IS_SLAVE = OFPMakeErrorCode(OFPET_BAD_REQUEST, 10),
  OFPBRC_BAD_PORT = OFPMakeErrorCode(OFPET_BAD_REQUEST, 11),
  OFPBRC_BAD_PACKET = OFPMakeErrorCode(OFPET_BAD_REQUEST, 12),
  OFPBRC_MULTIPART_BUFFER_OVERFLOW = OFPMakeErrorCode(OFPET_BAD_REQUEST, 13),

  OFPBAC_BAD_TYPE = OFPMakeErrorCode(OFPET_BAD_ACTION, 0),
  OFPBAC_BAD_LEN = OFPMakeErrorCode(OFPET_BAD_ACTION, 1),
  OFPBAC_BAD_EXPERIMENTER = OFPMakeErrorCode(OFPET_BAD_ACTION, 2),
  OFPBAC_BAD_EXP_TYPE = OFPMakeErrorCode(OFPET_BAD_ACTION, 3),
  OFPBAC_BAD_OUT_PORT = OFPMakeErrorCode(OFPET_BAD_ACTION, 4),
  OFPBAC_BAD_ARGUMENT = OFPMakeErrorCode(OFPET_BAD_ACTION, 5),
  OFPBAC_EPERM = OFPMakeErrorCode(OFPET_BAD_ACTION, 6),
  OFPBAC_TOO_MANY = OFPMakeErrorCode(OFPET_BAD_ACTION, 7),
  OFPBAC_BAD_QUEUE = OFPMakeErrorCode(OFPET_BAD_ACTION, 8),
  OFPBAC_BAD_OUT_GROUP = OFPMakeErrorCode(OFPET_BAD_ACTION, 9),
  OFPBAC_MATCH_INCONSISTENT = OFPMakeErrorCode(OFPET_BAD_ACTION, 10),
  OFPBAC_UNSUPPORTED_ORDER = OFPMakeErrorCode(OFPET_BAD_ACTION, 11),
  OFPBAC_BAD_TAG = OFPMakeErrorCode(OFPET_BAD_ACTION, 12),
  OFPBAC_BAD_SET_TYPE = OFPMakeErrorCode(OFPET_BAD_ACTION, 13),
  OFPBAC_BAD_SET_LEN = OFPMakeErrorCode(OFPET_BAD_ACTION, 14),
  OFPBAC_BAD_SET_ARGUMENT = OFPMakeErrorCode(OFPET_BAD_ACTION, 15),

  OFPBIC_UNKNOWN_INST = OFPMakeErrorCode(OFPET_BAD_INSTRUCTION, 0),
  OFPBIC_UNSUP_INST = OFPMakeErrorCode(OFPET_BAD_INSTRUCTION, 1),
  OFPBIC_BAD_TABLE_ID = OFPMakeErrorCode(OFPET_BAD_INSTRUCTION, 2),
  OFPBIC_UNSUP_METADATA = OFPMakeErrorCode(OFPET_BAD_INSTRUCTION, 3),
  OFPBIC_UNSUP_METADATA_MASK = OFPMakeErrorCode(OFPET_BAD_INSTRUCTION, 4),
  OFPBIC_BAD_EXPERIMENTER = OFPMakeErrorCode(OFPET_BAD_INSTRUCTION, 5),
  OFPBIC_BAD_EXP_TYPE = OFPMakeErrorCode(OFPET_BAD_INSTRUCTION, 6),
  OFPBIC_BAD_LEN = OFPMakeErrorCode(OFPET_BAD_INSTRUCTION, 7),
  OFPBIC_EPERM = OFPMakeErrorCode(OFPET_BAD_INSTRUCTION, 8),

  OFPBMC_BAD_TYPE = OFPMakeErrorCode(OFPET_BAD_MATCH, 0),
  OFPBMC_BAD_LEN = OFPMakeErrorCode(OFPET_BAD_MATCH, 1),
  OFPBMC_BAD_TAG = OFPMakeErrorCode(OFPET_BAD_MATCH, 2),
  OFPBMC_BAD_DL_ADDR_MASK = OFPMakeErrorCode(OFPET_BAD_MATCH, 3),
  OFPBMC_BAD_NW_ADDR_MASK = OFPMakeErrorCode(OFPET_BAD_MATCH, 4),
  OFPBMC_BAD_WILDCARDS = OFPMakeErrorCode(OFPET_BAD_MATCH, 5),
  OFPBMC_BAD_FIELD = OFPMakeErrorCode(OFPET_BAD_MATCH, 6),
  OFPBMC_BAD_VALUE = OFPMakeErrorCode(OFPET_BAD_MATCH, 7),
  OFPBMC_BAD_MASK = OFPMakeErrorCode(OFPET_BAD_MATCH, 8),
  OFPBMC_BAD_PREREQ = OFPMakeErrorCode(OFPET_BAD_MATCH, 9),
  OFPBMC_DUP_FIELD = OFPMakeErrorCode(OFPET_BAD_MATCH, 10),
  OFPBMC_EPERM = OFPMakeErrorCode(OFPET_BAD_MATCH, 11),

  OFPFMFC_UNKNOWN = OFPMakeErrorCode(OFPET_FLOW_MOD_FAILED, 0),
  OFPFMFC_TABLE_FULL = OFPMakeErrorCode(OFPET_FLOW_MOD_FAILED, 1),
  OFPFMFC_BAD_TABLE_ID = OFPMakeErrorCode(OFPET_FLOW_MOD_FAILED, 2),
  OFPFMFC_OVERLAP = OFPMakeErrorCode(OFPET_FLOW_MOD_FAILED, 3),
  OFPFMFC_EPERM = OFPMakeErrorCode(OFPET_FLOW_MOD_FAILED, 4),
  OFPFMFC_BAD_TIMEOUT = OFPMakeErrorCode(OFPET_FLOW_MOD_FAILED, 5),
  OFPFMFC_BAD_COMMAND = OFPMakeErrorCode(OFPET_FLOW_MOD_FAILED, 6),
  OFPFMFC_BAD_FLAGS = OFPMakeErrorCode(OFPET_FLOW_MOD_FAILED, 7),
  OFPFMFC_UNSUPPORTED = OFPMakeErrorCode(OFPET_FLOW_MOD_FAILED, 0x7f),  // 1.0

  OFPGMFC_GROUP_EXISTS = OFPMakeErrorCode(OFPET_GROUP_MOD_FAILED, 0),
  OFPGMFC_INVALID_GROUP = OFPMakeErrorCode(OFPET_GROUP_MOD_FAILED, 1),
  OFPGMFC_WEIGHT_UNSUPPORTED = OFPMakeErrorCode(OFPET_GROUP_MOD_FAILED, 2),
  OFPGMFC_OUT_OF_GROUPS = OFPMakeErrorCode(OFPET_GROUP_MOD_FAILED, 3),
  OFPGMFC_OUT_OF_BUCKETS = OFPMakeErrorCode(OFPET_GROUP_MOD_FAILED, 4),
  OFPGMFC_CHAINING_UNSUPPORTED = OFPMakeErrorCode(OFPET_GROUP_MOD_FAILED, 5),
  OFPGMFC_WATCH_UNSUPPORTED = OFPMakeErrorCode(OFPET_GROUP_MOD_FAILED, 6),
  OFPGMFC_LOOP = OFPMakeErrorCode(OFPET_GROUP_MOD_FAILED, 7),
  OFPGMFC_UNKNOWN_GROUP = OFPMakeErrorCode(OFPET_GROUP_MOD_FAILED, 8),
  OFPGMFC_CHAINED_GROUP = OFPMakeErrorCode(OFPET_GROUP_MOD_FAILED, 9),
  OFPGMFC_BAD_TYPE = OFPMakeErrorCode(OFPET_GROUP_MOD_FAILED, 10),
  OFPGMFC_BAD_COMMAND = OFPMakeErrorCode(OFPET_GROUP_MOD_FAILED, 11),
  OFPGMFC_BAD_BUCKET = OFPMakeErrorCode(OFPET_GROUP_MOD_FAILED, 12),
  OFPGMFC_BAD_WATCH = OFPMakeErrorCode(OFPET_GROUP_MOD_FAILED, 13),
  OFPGMFC_EPERM = OFPMakeErrorCode(OFPET_GROUP_MOD_FAILED, 14),

  OFPPMFC_BAD_PORT = OFPMakeErrorCode(OFPET_PORT_MOD_FAILED, 0),
  OFPPMFC_BAD_HW_ADDR = OFPMakeErrorCode(OFPET_PORT_MOD_FAILED, 1),
  OFPPMFC_BAD_CONFIG = OFPMakeErrorCode(OFPET_PORT_MOD_FAILED, 2),
  OFPPMFC_BAD_ADVERTISE = OFPMakeErrorCode(OFPET_PORT_MOD_FAILED, 3),
  OFPPMFC_EPERM = OFPMakeErrorCode(OFPET_PORT_MOD_FAILED, 4),

  OFPTMFC_BAD_TABLE = OFPMakeErrorCode(OFPET_TABLE_MOD_FAILED, 0),
  OFPTMFC_BAD_CONFIG = OFPMakeErrorCode(OFPET_TABLE_MOD_FAILED, 1),
  OFPTMFC_EPERM = OFPMakeErrorCode(OFPET_TABLE_MOD_FAILED, 2),

  OFPQOFC_BAD_PORT = OFPMakeErrorCode(OFPET_QUEUE_OP_FAILED, 0),
  OFPQOFC_BAD_QUEUE = OFPMakeErrorCode(OFPET_QUEUE_OP_FAILED, 1),
  OFPQOFC_EPERM = OFPMakeErrorCode(OFPET_QUEUE_OP_FAILED, 2),

  OFPSCFC_BAD_FLAGS = OFPMakeErrorCode(OFPET_SWITCH_CONFIG_FAILED, 0),
  OFPSCFC_BAD_LEN = OFPMakeErrorCode(OFPET_SWITCH_CONFIG_FAILED, 1),
  OFPSCFC_EPERM = OFPMakeErrorCode(OFPET_SWITCH_CONFIG_FAILED, 2),

  OFPRRFC_STALE = OFPMakeErrorCode(OFPET_ROLE_REQUEST_FAILED, 0),
  OFPRRFC_UNSUP = OFPMakeErrorCode(OFPET_ROLE_REQUEST_FAILED, 1),
  OFPRRFC_BAD_ROLE = OFPMakeErrorCode(OFPET_ROLE_REQUEST_FAILED, 2),

  OFPMMFC_UNKNOWN = OFPMakeErrorCode(OFPET_METER_MOD_FAILED, 0),
  OFPMMFC_METER_EXISTS = OFPMakeErrorCode(OFPET_METER_MOD_FAILED, 1),
  OFPMMFC_INVALID_METER = OFPMakeErrorCode(OFPET_METER_MOD_FAILED, 2),
  OFPMMFC_UNKNOWN_METER = OFPMakeErrorCode(OFPET_METER_MOD_FAILED, 3),
  OFPMMFC_BAD_COMMAND = OFPMakeErrorCode(OFPET_METER_MOD_FAILED, 4),
  OFPMMFC_BAD_FLAGS = OFPMakeErrorCode(OFPET_METER_MOD_FAILED, 5),
  OFPMMFC_BAD_RATE = OFPMakeErrorCode(OFPET_METER_MOD_FAILED, 6),
  OFPMMFC_BAD_BURST = OFPMakeErrorCode(OFPET_METER_MOD_FAILED, 7),
  OFPMMFC_BAD_BAND = OFPMakeErrorCode(OFPET_METER_MOD_FAILED, 8),
  OFPMMFC_BAD_BAND_VALUE = OFPMakeErrorCode(OFPET_METER_MOD_FAILED, 9),
  OFPMMFC_OUT_OF_METERS = OFPMakeErrorCode(OFPET_METER_MOD_FAILED, 10),
  OFPMMFC_OUT_OF_BANDS = OFPMakeErrorCode(OFPET_METER_MOD_FAILED, 11),

  OFPTFFC_BAD_TABLE = OFPMakeErrorCode(OFPET_TABLE_FEATURES_FAILED, 0),
  OFPTFFC_BAD_METADATA = OFPMakeErrorCode(OFPET_TABLE_FEATURES_FAILED, 1),
  OFPTFFC_BAD_TYPE = OFPMakeErrorCode(OFPET_TABLE_FEATURES_FAILED, 2),
  OFPTFFC_BAD_LEN = OFPMakeErrorCode(OFPET_TABLE_FEATURES_FAILED, 3),
  OFPTFFC_BAD_ARGUMENT = OFPMakeErrorCode(OFPET_TABLE_FEATURES_FAILED, 4),
  OFPTFFC_EPERM = OFPMakeErrorCode(OFPET_TABLE_FEATURES_FAILED, 5),

  OFPEC_UNKNOWN_FLAG = 0x80000000
};

constexpr OFPErrorCode OFPErrorCodeMake(UInt16 type, UInt16 code) {
  return static_cast<OFPErrorCode>(OFPMakeErrorCode(type, code));
}

constexpr OFPErrorType OFPErrorCodeGetType(OFPErrorCode code) {
  return static_cast<OFPErrorType>((code >> 16) & 0xffff);
}

constexpr UInt16 OFPErrorCodeGetCode(OFPErrorCode code) {
  return (code & 0xffff);
}

constexpr bool OFPErrorCodeIsUnknown(OFPErrorCode code) {
  return (code & OFPEC_UNKNOWN_FLAG) != 0;
}

constexpr OFPErrorCode OFPErrorCodeSetType(OFPErrorCode code, UInt16 type) {
  return OFPErrorCodeMake(type, code & 0xffff);
}

OFPErrorType OFPErrorTypeFromValue(UInt8 version, UInt16 type, UInt16 code);
UInt16 OFPErrorTypeToValue(UInt8 version, OFPErrorCode code);

OFPErrorCode OFPErrorCodeFromValue(UInt8 version, UInt16 type, UInt16 code);
UInt16 OFPErrorCodeToValue(UInt8 version, OFPErrorCode code);

}  // namespace ofp

#endif  // OFP_CONSTANTS_ERROR_H_

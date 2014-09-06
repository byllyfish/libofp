//  ===== ---- ofp/api/rpcevents.h -------------------------*- C++ -*- =====  //
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
/// \brief Implements ....
//  ===== ------------------------------------------------------------ =====  //

#ifndef OFP_API_RPCEVENTS_H_
#define OFP_API_RPCEVENTS_H_

#include "ofp/yaml/yllvm.h"
#include "ofp/datapathid.h"
#include "ofp/yaml/ydatapathid.h"
#include "ofp/driver.h"
#include "ofp/padding.h"
#include "ofp/yaml/ybytelist.h"
#include "ofp/yaml/yaddress.h"
#include "ofp/yaml/encoder.h"

namespace ofp {
namespace api {

/// Indicates that `id` is missing from RPC message.
const UInt64 RPC_ID_MISSING = 0xffffffffffffffffUL;

/// RPC Methods
enum RpcMethod : UInt32 {
  METHOD_OPEN,          // ofp.open
  METHOD_CLOSE,         // ofp.close
  METHOD_SEND,          // ofp.send
  METHOD_SET_TIMER,     // ofp.set_timer
  METHOD_CONFIG,        // ofp.config
  METHOD_DATAPATH,      // ofp.datapath
  METHOD_MESSAGE,       // ofp.message
  METHOD_MESSAGE_ERROR, // ofp.message_error
  METHOD_TIMER,         // ofp.timer
  METHOD_UNSUPPORTED
};

/// JSON-RPC error codes.
enum RpcErrorCode {
  ERROR_CODE_INVALID_REQUEST = -32600
};

OFP_BEGIN_IGNORE_PADDING

/// Represents the general JSON-RPC error response.
struct RpcErrorResponse {
  explicit RpcErrorResponse(UInt64 ident) : id{ident} {}
  std::string toJson();

  struct Error {
    int code;
    std::string message;
  };

  UInt64 id;
  Error error;
};

//-----------------//
// o f p . o p e n //
//-----------------//

/// Represents a RPC request to open a connection (METHOD_OPEN)
struct RpcOpen {
  explicit RpcOpen(UInt64 ident) : id{ident} {}

  struct Params {
    /// Endpoint to connect to or listen on.
    IPv6Endpoint endpoint;
    /// Array of options passed to open.
    std::vector<std::string> options;
  };

  UInt64 id;
  Params params;
};

/// Represents a RPC response to open a connection (METHOD_OPEN)
struct RpcOpenResponse {
  explicit RpcOpenResponse(UInt64 ident) : id{ident} {}
  std::string toJson();

  struct Result {
    /// Connection ID of opened connection.
    UInt64 connId;
    /// Status of open connection: `listening` or `connecting`.
    std::string status;
  };

  UInt64 id;
  Result result;
};

//-------------------//
// o f p . c l o s e //
//-------------------//

/// Represents a RPC request to close a connection (METHOD_CLOSE)
struct RpcClose {
  explicit RpcClose(UInt64 ident) : id{ident} {}

  struct Params {
    /// Connection ID to close.
    UInt64 connId;
  };

  UInt64 id;
  Params params;
};

/// Represents a RPC response to close a connection (METHOD_CLOSE)
struct RpcCloseResponse {
  /// Count of closed connections (0 or 1).
  UInt32 count;
};

//---------------------------//
// o f p . s e t _ t i m e r //
//---------------------------//

/// Represents a RPC request to set a timer (METHOD_SET_TIMER)
struct RpcSetTimer {
  explicit RpcSetTimer(UInt64 ident) : id{ident} {}

  struct Params {
    DatapathID datapathId;
    UInt32 timerId;
    UInt32 timeout;
  };

  UInt64 id;
  Params params;
};

/// Represents a RPC response to set a timer (METHOD_SET_TIMER).
struct RpcSetTimerResponse {
  explicit RpcSetTimerResponse(UInt64 ident) : id{ident} {}
  std::string toJson();

  struct Result {
    DatapathID datapathId;
    UInt32 timerId;
    UInt32 timeout;
  };

  UInt64 id;
  Result result;
};

//-----------------//
// o f p . s e n d //
//-----------------//

/// Represents a RPC request to send a message to datapath (METHOD_SEND).
struct RpcSend {
  explicit RpcSend(UInt64 ident, yaml::Encoder::ChannelFinder finder) : id{ident}, params{finder} {}

  UInt64 id;
  yaml::Encoder params;
};

/// Represnts a RPC response to send a message to datapath (METHOD_SEND).
struct RpcSendResponse {
  explicit RpcSendResponse(UInt64 ident) : id{ident} {}
  std::string toJson();

  struct Result {
    ByteRange data;
  };

  UInt64 id;
  Result result;
};

//---------------------//
// o f p . c o n f i g //
//---------------------//

/// Represents a RPC request to configure some options (METHOD_CONFIG).
struct RpcConfig {
  explicit RpcConfig(UInt64 ident) : id{ident} {}

  struct Params {
    std::vector<std::string> options;
  };

  UInt64 id;
  Params params;
};

/// Represents a RPC response to configure some options (METHOD_CONFIG).
struct RpcConfigResponse {
  explicit RpcConfigResponse(UInt64 ident) : id{ident} {}
  std::string toJson();

  struct Result {
    std::vector<std::string> options;
  };

  UInt64 id;
  Result result;
};

//-------------------------//
// o f p . d a t a p a t h //
//-------------------------//

/// Represents a RPC notification about a datapath (METHOD_DATAPATH)
struct RpcDatapath {
  std::string toJson();

  struct Params {
    DatapathID datapathId;
    IPv6Endpoint endpoint;
    std::string status;
    UInt8 version{};
  };

  Params params;
};

//-------------------//
// o f p . t i m e r //
//-------------------//

/// Represents a RPC timer notification (METHOD_TIMER).
struct RpcTimer {
  std::string toJson();

  struct Params {
    DatapathID datapathId;
    UInt32 timerId;
  };

  UInt64 id;
  Params params;
};

//-----------------------------------//
// o f p . m e s s a g e _ e r r o r //
//-----------------------------------//

/// Represents a RPC notification about an incoming message error (METHOD_MESSAGE_ERROR).
struct RpcMessageError {
  std::string toJson();

  struct Params {
    DatapathID datapathId;
    std::string error;
    ByteRange data;
  };

  Params params;
};

OFP_END_IGNORE_PADDING

}  // namespace api
}  // namespace ofp

namespace llvm {
namespace yaml {

template <>
struct ScalarEnumerationTraits<ofp::api::RpcMethod> {
  static void enumeration(IO &io, ofp::api::RpcMethod &value) {
    io.enumCase(value, "ofp.open", ofp::api::METHOD_OPEN);
    io.enumCase(value, "ofp.close", ofp::api::METHOD_CLOSE);
    io.enumCase(value, "ofp.send", ofp::api::METHOD_SEND);
    io.enumCase(value, "ofp.config", ofp::api::METHOD_CONFIG);
    io.enumCase(value, "ofp.set_timer", ofp::api::METHOD_SET_TIMER);
    io.enumCase(value, "ofp.datapath", ofp::api::METHOD_DATAPATH);
    io.enumCase(value, "ofp.message", ofp::api::METHOD_MESSAGE);
    io.enumCase(value, "ofp.message_error", ofp::api::METHOD_MESSAGE_ERROR);
    io.enumCase(value, "ofp.timer", ofp::api::METHOD_TIMER);
  }
};

template <>
struct MappingTraits<ofp::api::RpcOpen::Params> {
  static void mapping(IO &io, ofp::api::RpcOpen::Params &params) {
    io.mapRequired("endpoint", params.endpoint);
    io.mapOptional("options", params.options);
  }
};

template <>
struct MappingTraits<ofp::api::RpcClose::Params> {
  static void mapping(IO &io, ofp::api::RpcClose::Params &params) {
    io.mapRequired("conn_id", params.connId);
  }
};

template <>
struct MappingTraits<ofp::api::RpcSetTimer::Params> {
  static void mapping(IO &io, ofp::api::RpcSetTimer::Params &params) {
    io.mapRequired("datapath_id", params.datapathId);
    io.mapRequired("timer_id", params.timerId);
    io.mapRequired("timeout", params.timeout);
  }
};

template <>
struct MappingTraits<ofp::api::RpcTimer> {
  static void mapping(IO &io, ofp::api::RpcTimer &response) {
    ofp::api::RpcMethod method = ofp::api::METHOD_TIMER;
    io.mapRequired("method", method);
    io.mapRequired("params", response.params);
  }
};

template <>
struct MappingTraits<ofp::api::RpcTimer::Params> {
  static void mapping(IO &io, ofp::api::RpcTimer::Params &params) {
    io.mapRequired("datapath_id", params.datapathId);
    io.mapRequired("timer_id", params.timerId);
  }
};

template <>
struct MappingTraits<ofp::api::RpcConfig::Params> {
  static void mapping(IO &io, ofp::api::RpcConfig::Params &params) {
    io.mapRequired("options", params.options);
  }
};

template <>
struct MappingTraits<ofp::api::RpcOpenResponse> {
  static void mapping(IO &io, ofp::api::RpcOpenResponse &response) {
    io.mapRequired("id", response.id);
    io.mapRequired("result", response.result);
  }
};

template <>
struct MappingTraits<ofp::api::RpcOpenResponse::Result> {
  static void mapping(IO &io, ofp::api::RpcOpenResponse::Result &result) {
    io.mapRequired("conn_id", result.connId);
  }
};

template <>
struct MappingTraits<ofp::api::RpcSendResponse> {
  static void mapping(IO &io, ofp::api::RpcSendResponse &response) {
    io.mapRequired("id", response.id);
    io.mapRequired("result", response.result);
  }
};

template <>
struct MappingTraits<ofp::api::RpcSendResponse::Result> {
  static void mapping(IO &io, ofp::api::RpcSendResponse::Result &result) {
    io.mapRequired("data", result.data);
  }
};

template <>
struct MappingTraits<ofp::api::RpcErrorResponse> {
  static void mapping(IO &io, ofp::api::RpcErrorResponse &response) {
    io.mapRequired("id", response.id);
    io.mapRequired("error", response.error);
  }
};

template <>
struct MappingTraits<ofp::api::RpcErrorResponse::Error> {
  static void mapping(IO &io, ofp::api::RpcErrorResponse::Error &error) {
    io.mapRequired("code", error.code);
    io.mapRequired("message", error.message);
  }
};

template <>
struct MappingTraits<ofp::api::RpcDatapath> {
  static void mapping(IO &io, ofp::api::RpcDatapath &response) {
    ofp::api::RpcMethod method = ofp::api::METHOD_DATAPATH;
    io.mapRequired("method", method);
    io.mapRequired("params", response.params);
  }
};

template <>
struct MappingTraits<ofp::api::RpcDatapath::Params> {
  static void mapping(IO &io, ofp::api::RpcDatapath::Params &params) {
    io.mapRequired("datapath_id", params.datapathId);
    io.mapRequired("version", params.version);
    io.mapRequired("endpoint", params.endpoint);
    io.mapRequired("status", params.status);
  }
};

template <>
struct MappingTraits<ofp::api::RpcMessageError> {
  static void mapping(IO &io, ofp::api::RpcMessageError &response) {
    ofp::api::RpcMethod method = ofp::api::METHOD_MESSAGE_ERROR;
    io.mapRequired("method", method);
    io.mapRequired("params", response.params);
  }
};

template <>
struct MappingTraits<ofp::api::RpcMessageError::Params> {
  static void mapping(IO &io, ofp::api::RpcMessageError::Params &params) {
    io.mapRequired("datapath_id", params.datapathId);
    io.mapRequired("error", params.error);
    io.mapRequired("data", params.data);
  }
};


}  // namespace yaml
}  // namespace llvm

#endif  // OFP_API_RPCEVENTS_H_

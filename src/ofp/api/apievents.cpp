//  ===== ---- ofp/api/apievents.cpp -----------------------*- C++ -*- =====  //
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
/// \brief Implements api API event classes.
//  ===== ------------------------------------------------------------ =====  //

#include "ofp/api/apievents.h"
#include "ofp/yaml/outputjson.h"

namespace {

/// Converts event to a YAML representation.
template <class Type>
std::string convertToString(Type *event, bool useJson) {
  std::string yaml;
  llvm::raw_string_ostream rss(yaml);
  if (useJson) {
    ofp::yaml::OutputJson yout{rss};
    yout << *event;
  } else {
    llvm::yaml::Output yout{rss};
    yout << *event;
  }
  (void)rss.str();
  return yaml;
}

}  // namespace

/// \returns YAML representation of LIBOFP_LOOPBACK event.
std::string ofp::api::ApiLoopback::toString(bool useJson) {
  return convertToString(this, useJson);
}

/// \returns YAML representation of LIBOFP_LISTEN_REPLY event.
std::string ofp::api::ApiListenReply::toString(bool useJson) {
  return convertToString(this, useJson);
}

/// \returns YAML representation of LIBOFP_CONNECT_REPLY event.
std::string ofp::api::ApiConnectReply::toString(bool useJson) {
  return convertToString(this, useJson);
}

/// \returns YAML representation of LIBOFP_YAML_ERROR event.
std::string ofp::api::ApiYamlError::toString(bool useJson) {
  return convertToString(this, useJson);
}

/// \returns YAML representation of LIBOFP_DECODE_ERROR event.
std::string ofp::api::ApiDecodeError::toString(bool useJson) {
  return convertToString(this, useJson);
}

/// \returns YAML representation of LIBOFP_DATAPATH_UP event.
std::string ofp::api::ApiDatapathUp::toString(bool useJson) {
  return convertToString(this, useJson);
}

/// \returns YAML representation of LIBOFP_DATAPATH_DOWN event.
std::string ofp::api::ApiDatapathDown::toString(bool useJson) {
  return convertToString(this, useJson);
}

/// \returns YAML representation of LIBOFP_TIMER event.
std::string ofp::api::ApiTimer::toString(bool useJson) {
  return convertToString(this, useJson);
}
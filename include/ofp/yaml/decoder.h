// Copyright 2014-present Bill Fisher. All rights reserved.

#ifndef OFP_YAML_DECODER_H_
#define OFP_YAML_DECODER_H_

#include "ofp/yaml/yllvm.h"
#include "ofp/yaml/ybyteorder.h"
#include "ofp/yaml/ydatapathid.h"
#include "ofp/yaml/ytimestamp.h"
#include "ofp/yaml/yaddress.h"
#include "ofp/message.h"
#include "ofp/channel.h"
#include "ofp/messageinfo.h"

namespace ofp {
namespace yaml {

class Decoder {
 public:
  explicit Decoder(const Message *msg, bool useJsonFormat = false,
                   bool includePktMatch = false);

  const llvm::StringRef result() const { return result_; }

  const std::string &error() const { return error_; }

 private:
  const Message *msg_;
  llvm::SmallString<1024> result_;
  std::string error_;

  explicit Decoder(const Message *msg, const Decoder *decoder);

  bool decodeMsg(llvm::yaml::IO &io);
  bool decodeRequestForward(llvm::yaml::IO &io, const Message *msg);

  friend struct llvm::yaml::MappingTraits<ofp::yaml::Decoder>;
  friend void DecodeRecursively(llvm::yaml::IO &io, const char *key,
                                const Message *msg);
};

}  // namespace yaml
}  // namespace ofp

namespace llvm {
namespace yaml {

template <>
struct MappingTraits<ofp::yaml::Decoder> {
  static void mapping(IO &io, ofp::yaml::Decoder &decoder) {
    using namespace ofp;

    Header header = *decoder.msg_->header();
    assert(header.length() == decoder.msg_->size());

    ofp::yaml::MessageType msgType{decoder.msg_->type(),
                                   decoder.msg_->subtype()};
    io.mapRequired("type", msgType);

    if (msgType.type == OFPT_MULTIPART_REQUEST ||
        msgType.type == OFPT_MULTIPART_REPLY) {
      OFPMultipartFlags flags = decoder.msg_->flags();
      io.mapRequired("flags", flags);
    }

    io.mapRequired("xid", header.xid_);
    io.mapRequired("version", header.version_);

    Timestamp time = decoder.msg_->time();
    if (time.valid()) {
      io.mapRequired("time", time);
    }

    Channel *source = decoder.msg_->source();
    if (source) {
      Hex64 connId = source->connectionId();
      if (connId) {
        io.mapRequired("conn_id", connId);
      }

      DatapathID dpid = source->datapathId();
      if (!dpid.empty()) {
        io.mapRequired("datapath_id", dpid);
      }

      Hex8 auxID = source->auxiliaryId();
      if (auxID) {
        io.mapRequired("auxiliary_id", auxID);
      }
    }

    const MessageInfo *info = decoder.msg_->info();
    if (info) {
      UInt64 sessionId = info->sessionId();
      IPv6Endpoint src = info->source();
      IPv6Endpoint dst = info->dest();
      io.mapRequired("_session", sessionId);
      io.mapRequired("_source", src);
      io.mapRequired("_dest", dst);
    }

    if (!decoder.decodeMsg(io)) {
      decoder.error_ = "Invalid data.";
    }
  }
};

}  // namespace yaml
}  // namespace llvm

#endif  // OFP_YAML_DECODER_H_

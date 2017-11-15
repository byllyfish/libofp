// Copyright (c) 2015-2017 William W. Fisher (at gmail dot com)
// This file is distributed under the MIT License.

#ifndef OFP_RPC_RPCCONNECTIONSTDIO_H_
#define OFP_RPC_RPCCONNECTIONSTDIO_H_

#include "ofp/rpc/rpcconnection.h"
#include "ofp/sys/asio_utils.h"
#include "ofp/sys/handler_allocator.h"

namespace ofp {
namespace rpc {

OFP_BEGIN_IGNORE_PADDING

class RpcConnectionStdio final : public RpcConnection {
 public:
  RpcConnectionStdio(RpcServer *server, asio::posix::stream_descriptor input,
                     asio::posix::stream_descriptor output);

  void asyncAccept() override;
  void close() override;

 protected:
  void writeEvent(llvm::StringRef msg, bool ofp_message=false) override;
  void asyncRead() override;

 private:
  asio::posix::stream_descriptor input_;
  asio::posix::stream_descriptor output_;
  asio::streambuf streambuf_;
  asio::steady_timer metricTimer_;
  Big32 hdrBuf_;
  std::string eventBuf_;
  sys::handler_allocator allocator_;

  // Use a two buffer strategy for async-writes. We queue up data in one
  // buffer while we're in the process of writing the other buffer.
  ByteList outgoing_[2];
  int outgoingIdx_ = 0;
  bool writing_ = false;

  void asyncReadHeader();
  void asyncReadMessage(size_t msgLength);

  void asyncWrite();
  void asyncMetrics(Milliseconds interval);
  void logMetrics();
};

OFP_END_IGNORE_PADDING

}  // namespace rpc
}  // namespace ofp

#endif  // OFP_RPC_RPCCONNECTIONSTDIO_H_

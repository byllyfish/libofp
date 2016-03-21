// Copyright (c) 2015-2016 William W. Fisher (at gmail dot com)
// This file is distributed under the MIT License.

#ifndef OFP_DEFAULTHANDSHAKE_H_
#define OFP_DEFAULTHANDSHAKE_H_

#include "ofp/channellistener.h"
#include "ofp/driver.h"
#include "ofp/protocolversions.h"

namespace ofp {

namespace sys {
class Connection;
}  // namespace sys

OFP_BEGIN_IGNORE_PADDING

class DefaultHandshake : public ChannelListener {
 public:
  DefaultHandshake(sys::Connection *channel, ChannelOptions options,
                   ProtocolVersions versions, Factory listenerFactory);

  void onChannelUp(Channel *channel) override;
  void onChannelDown(Channel *channel) override;
  void onMessage(const Message *message) override;

  ChannelOptions options() const { return options_; }
  ProtocolVersions versions() const { return versions_; }

  void setStartingXid(UInt32 xid) { startingXid_ = xid; }
  void setConnection(sys::Connection *channel) { channel_ = channel; }

 private:
  sys::Connection *channel_;
  ProtocolVersions versions_;
  Factory listenerFactory_;
  ChannelOptions options_;
  UInt32 startingXid_ = 0;

  void onHello(const Message *message);
  void onFeaturesReply(const Message *message);
  void onError(const Message *message);

  void installNewChannelListener(const Message *message);
  void clearChannelListener();
};

OFP_END_IGNORE_PADDING

}  // namespace ofp

#endif  // OFP_DEFAULTHANDSHAKE_H_
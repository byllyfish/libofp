// Copyright (c) 2015-2017 William W. Fisher (at gmail dot com)
// This file is distributed under the MIT License.

#ifndef OFP_SYS_DEFAULTHANDSHAKE_H_
#define OFP_SYS_DEFAULTHANDSHAKE_H_

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
  void onMessage(Message *message) override;
  bool onTickle(Channel *channel, TimePoint now) override;

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
  TimePoint timeStarted_;

  void onHello(const Message *message);
  void onFeaturesReply(Message *message);
  void onError(const Message *message);

  void installNewChannelListener(Message *message);
  void clearChannelListener();

  bool wantFeatures() const { return (options_ & ChannelOptions::FEATURES_REQ) != 0; }
};

OFP_END_IGNORE_PADDING

}  // namespace ofp

#endif  // OFP_SYS_DEFAULTHANDSHAKE_H_

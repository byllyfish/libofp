// Copyright (c) 2015-2016 William W. Fisher (at gmail dot com)
// This file is distributed under the MIT License.

#include "ofp/sys/defaulthandshake.h"
#include <sstream>
#include "ofp/constants.h"
#include "ofp/error.h"
#include "ofp/featuresreply.h"
#include "ofp/headeronly.h"
#include "ofp/hello.h"
#include "ofp/log.h"
#include "ofp/message.h"
#include "ofp/sys/connection.h"
#include "ofp/sys/engine.h"

using namespace ofp;
using sys::Connection;

const Milliseconds kControllerKeepAliveTimeout = 10000_ms;
const Milliseconds kRawKeepAliveTimeout = 6000_ms;

DefaultHandshake::DefaultHandshake(Connection *channel, ChannelOptions options,
                                   ProtocolVersions versions,
                                   Factory listenerFactory)
    : channel_{channel},
      versions_{versions},
      listenerFactory_{listenerFactory},
      options_{options} {}

void DefaultHandshake::onChannelUp(Channel *channel) {
  assert(channel == channel_);

  channel->setStartingXid(startingXid_);

  HelloBuilder msg{versions_};
  msg.send(channel_);
}

void DefaultHandshake::onChannelDown(Channel *channel) {
  log::warning("DefaultHandshake: Channel down before handshake could complete",
               std::make_pair("connid", channel->connectionId()));
}

void DefaultHandshake::onMessage(const Message *message) {
  switch (message->type()) {
    case Hello::type():
      onHello(message);
      break;

    case FeaturesReply::type():
      onFeaturesReply(message);
      break;

    case Error::type():
      onError(message);
      break;

    default:
      log::warning("DefaultHandshake: Ignored message type", message->type(),
                   std::make_pair("connid", message->source()->connectionId()));
      break;
  }
}

void DefaultHandshake::onHello(const Message *message) {
  const Hello *msg = Hello::cast(message);
  if (!msg)
    return;

  const Header *header = msg->msgHeader();
  UInt8 msgVersion = header->version();
  ProtocolVersions msgVersionSet = msg->protocolVersions();
  UInt8 version = versions_.negotiateVersion(msgVersion, msgVersionSet);

  if (version == 0) {
    // If there are no versions in common, send an error and terminate the
    // connection.
    channel_->setVersion(versions_.highestVersion());

    std::stringstream sstr;
    sstr << "Incompatible OpenFlow version: ";
    sstr << static_cast<int>(msgVersion) << ' ' << msgVersionSet.toString();
    sstr << " Supported versions: " << versions_.toString();
    auto explanation = sstr.str();

    log::warning(explanation,
                 std::make_pair("connid", channel_->connectionId()));
    channel_->engine()->alert(channel_, explanation, {header, sizeof(*header)});

    message->replyError(OFPHFC_INCOMPATIBLE, explanation);
    channel_->shutdown();
    return;
  }

  log::debug("Negotiated version is", static_cast<int>(version));

  channel_->setVersion(version);

  log::info("OpenFlow version:", static_cast<int>(msgVersion), "Peer versions:",
            msg->protocolVersions().toString(),
            std::make_pair("connid", channel_->connectionId()));

  if ((options_ & ChannelOptions::FEATURES_REQ) != 0) {
    channel_->setKeepAliveTimeout(kControllerKeepAliveTimeout);

    FeaturesRequestBuilder reply{};
    reply.send(channel_);

  } else {
    channel_->setKeepAliveTimeout(kRawKeepAliveTimeout);
    installNewChannelListener(nullptr);
  }

  // TODO(bfish): handle case where we reconnected with a startingVersion of 1
  // but the other end supports a higher version number.
}

void DefaultHandshake::onFeaturesReply(const Message *message) {
  // Only a controller should be receiving a features reply message.
  if ((options_ & ChannelOptions::FEATURES_REQ) == 0) {
    return;
  }

  const FeaturesReply *msg = FeaturesReply::cast(message);
  if (!msg)
    return;  // FIXME log

  // Registering the connection allows us to attach auxiliary connections to
  // their main connections. A main connection (auxiliary_id == 0) cannot use
  // a UDP transport.

  if (channel_->postDatapath(msg->datapathId(), msg->auxiliaryId())) {
    if ((options_ & ChannelOptions::AUXILIARY) != 0 &&
        channel_->mainConnection() != channel_) {
      assert(msg->auxiliaryId() != 0);
      // This is an auxiliary connection, clear its channel listener. Note
      // that we do not pass the (auxiliary) FeaturesReply message to the
      // channel.
      clearChannelListener();

    } else {
      installNewChannelListener(message);
    }

  } else {
    channel_->shutdown();
  }
}

void DefaultHandshake::onError(const Message *message) {
  // FIXME log it
}

void DefaultHandshake::installNewChannelListener(const Message *message) {
  assert(channel_->channelListener() == this);

  if (listenerFactory_) {
    ChannelListener *newListener = listenerFactory_();
    channel_->setChannelListener(newListener);
    newListener->onChannelUp(channel_);

    if (message)
      newListener->onMessage(message);

    ChannelListener::dispose(this);

  } else {
    clearChannelListener();
  }
}

void DefaultHandshake::clearChannelListener() {
  assert(channel_->channelListener() == this);

  channel_->setChannelListener(nullptr);
  ChannelListener::dispose(this);
}
// Copyright 2014-present Bill Fisher. All rights reserved.

#ifndef OFP_RPC_RPCSERVER_H_
#define OFP_RPC_RPCSERVER_H_

#include <map>
#include "ofp/driver.h"
#include "ofp/datapathid.h"

namespace ofp {

class ByteRange;

namespace rpc {

class RpcConnection;
class RpcSession;

struct RpcListen;
struct RpcConnect;
struct RpcClose;
struct RpcSend;
struct RpcListConns;
struct RpcAddIdentity;
struct RpcDescription;

OFP_BEGIN_IGNORE_PADDING

/// \brief Implements a server that lets a client control and monitor an
/// OpenFlow driver over stdin/stdout.
/// The driver is controlled using YAML messages.
class RpcServer {
 public:
  RpcServer(Driver *driver, int inputFD, int outputFD,
            Channel *defaultChannel = nullptr);
  RpcServer(Driver *driver, RpcSession *session,
            Channel *defaultChannel = nullptr);
  ~RpcServer();

  /// Close the control connection.
  void close();

  // Called by RpcConnection to update oneConn_.
  void onConnect(RpcConnection *conn);
  void onDisconnect(RpcConnection *conn);

  void onRpcListen(RpcConnection *conn, RpcListen *open);
  void onRpcConnect(RpcConnection *conn, RpcConnect *connect);
  void onRpcClose(RpcConnection *conn, RpcClose *close);
  void onRpcSend(RpcConnection *conn, RpcSend *send);
  void onRpcListConns(RpcConnection *conn, RpcListConns *list);
  void onRpcAddIdentity(RpcConnection *conn, RpcAddIdentity *add);
  void onRpcDescription(RpcConnection *conn, RpcDescription *desc);

  // These methods are used to bridge RpcChannelListeners to RpcConnections.
  void onChannelUp(Channel *channel);
  void onChannelDown(Channel *channel);
  void onMessage(Channel *channel, const Message *message);

  Channel *findDatapath(const DatapathID &datapathId, UInt64 connId);

  sys::Engine *engine() { return engine_; }

 private:
  sys::Engine *engine_;
  RpcConnection *oneConn_ = nullptr;
  Channel *defaultChannel_ = nullptr;

  static void connectResponse(RpcConnection *conn, UInt64 id, UInt64 connId,
                              const std::error_code &err);
  static void alertCallback(Channel *channel, const std::string &alert, const ByteRange &data, void *context);
  static std::string softwareVersion();
};

OFP_END_IGNORE_PADDING

}  // namespace rpc
}  // namespace ofp

#endif  // OFP_RPC_RPCSERVER_H_

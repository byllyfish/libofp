//  ===== ---- ofp/sys/udp_server.h ------------------------*- C++ -*- =====  //
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
/// \brief Defines the UDP_Server class.
//  ===== ------------------------------------------------------------ =====  //

#ifndef OFP_SYS_UDP_SERVER_H_
#define OFP_SYS_UDP_SERVER_H_

#include "ofp/types.h"
#include "ofp/sys/asio_utils.h"
#include "ofp/ipv6address.h"
#include "ofp/message.h"
#include "ofp/driver.h"
#include <unordered_map>

namespace ofp {
namespace sys {

class Engine;
class UDP_Connection;

OFP_BEGIN_IGNORE_PADDING

class UDP_Server {
public:

	enum { MaxDatagramLength = 2000 };  // FIXME?

	UDP_Server(Engine *engine, ChannelMode mode, const IPv6Endpoint &endpt, ProtocolVersions versions, UInt64 connId, std::error_code &error);
	~UDP_Server();

	IPv6Endpoint localEndpoint() const;

	// Used by UDP_Connections to manage their lifetimes.
	void add(UDP_Connection *conn);
	void remove(UDP_Connection *conn);

	void write(const void *data, size_t length);
	void flush(udp::endpoint endpt);
	
	Engine *engine() const { return engine_; }

private:
	using ConnectionMap = std::unordered_map<IPv6Endpoint,UDP_Connection*>;

	Engine *engine_;
	ChannelMode mode_;
	ProtocolVersions versions_;
	udp::socket socket_;
	udp::endpoint sender_;
	Message message_;
	ConnectionMap connMap_;
	UInt64 connId_ = 0;
	bool shuttingDown_ = false;
	log::Lifetime lifetime_{"UDP_Server"};

	void listen(const IPv6Endpoint &localEndpt, std::error_code &error);
	void asyncReceive();
	void asyncSend();

	void dispatchMessage();
};

OFP_END_IGNORE_PADDING

}  // namespace sys
}  // namespace ofp

#endif  // OFP_SYS_UDP_SERVER_H_

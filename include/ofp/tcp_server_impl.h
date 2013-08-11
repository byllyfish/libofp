#ifndef OFP_TCP_SERVER_IMPL_H
#define OFP_TCP_SERVER_IMPL_H

#include "ofp/types.h"
#include "ofp/boost_impl.h"
#include "ofp/tcp_connection_impl.h"

namespace ofp {  // <namespace ofp>
namespace impl { // <namespace impl>

class Driver_Impl;

class TCP_Server {
public:

    TCP_Server(Driver_Impl *driver, Driver::Role role, const tcp::endpoint &endpt, ProtocolVersions versions, ChannelListener::Factory listenerFactory);
    ~TCP_Server();

    Driver_Impl *driver() const { return driver_; }

private:
	Driver_Impl *driver_;
    tcp::acceptor acceptor_;
    tcp::socket socket_;
    Driver::Role role_;
    ProtocolVersions versions_;
    ChannelListener::Factory factory_;

    void asyncAccept();
};

} // </namespace impl>
} // </namespace ofp>

#endif // OFP_TCP_SERVER_IMPL_H

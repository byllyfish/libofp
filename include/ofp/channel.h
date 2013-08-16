#ifndef OFP_CHANNEL_H
#define OFP_CHANNEL_H

#include "ofp/writable.h"
#include "ofp/ipv6address.h"
#include <chrono>

namespace ofp { // <namespace ofp>

class Driver;
class Features;
class ChannelListener;

class Channel : public Writable {
public:
	virtual ~Channel();
	
	virtual Driver *driver() const = 0;
	virtual const Features &features() const = 0;
	virtual IPv6Address remoteAddress() const = 0;
	virtual UInt16 remotePort() const = 0;

	virtual ChannelListener *channelListener() const = 0;
	virtual void setChannelListener(ChannelListener *listener) = 0;

	virtual void scheduleTimer(UInt32 timerID, std::chrono::milliseconds when) = 0;
	virtual void cancelTimer(UInt32 timerID) = 0;

	// The following methods are provided for OpenFlow agents.
	// The failure to open an auxiliary channel will be reported to the main listener via onException.
	
	enum class Transport {
		TCP,
		UDP,
		TLS,
		DTLS
	};

	virtual Transport transport() const = 0;
	virtual void openAuxChannel(UInt8 auxID, Transport transport) = 0;
	virtual Channel *findAuxChannel(UInt8 auxID) const = 0;

};


std::ostream &operator<<(std::ostream &os, Channel *channel);


} // </namespace ofp>


inline std::ostream &ofp::operator<<(std::ostream &os, ofp::Channel *channel)
{
	return os << "[Channel to=" << channel->remoteAddress() << ':' << channel->remotePort() << ']';
}

#endif // OFP_CHANNEL_H

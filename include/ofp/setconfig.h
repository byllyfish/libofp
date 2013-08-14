#ifndef OFP_SETCONFIG_H
#define OFP_SETCONFIG_H

#include "ofp/header.h"
#include "ofp/message.h"

namespace ofp { // <namespace ofp>

class SetConfig {
public:
	enum { Type = OFPT_SET_CONFIG };

	static const SetConfig *cast(const Message *message);

	SetConfig() : header_{Type} {}

	UInt16 flags() const { return flags_; }
	UInt16 missSendLen() const { return missSendLen_; }

private:
	Header header_;
	Big16 flags_;
	Big16 missSendLen_;

	bool validateLength(size_t length) const;

	friend class SetConfigBuilder;
};

static_assert(sizeof(SetConfig) == 12, "Unexpected size.");
static_assert(IsStandardLayout<SetConfig>(), "Expected standard layout.");

class SetConfigBuilder {
public:
	SetConfigBuilder() = default;

	void setFlags(UInt16 flags);
	void setMissSendLen(UInt16 missSendLen);
	
	UInt32 send(Writable *channel);

private:
	SetConfig msg_;
};

} // </namespace ofp>

#endif // OFP_SETCONFIG_H

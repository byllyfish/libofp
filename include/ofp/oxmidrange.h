// Copyright 2014-present Bill Fisher. All rights reserved.

#ifndef OFP_OXMIDRANGE_H_
#define OFP_OXMIDRANGE_H_

#include "ofp/protocolrange.h"
#include "ofp/oxmid.h"

namespace ofp {

using OXMIDIterator = ProtocolIterator<OXMID, ProtocolIteratorType::OXMID>;
using OXMIDRange = ProtocolRange<OXMIDIterator>;

}  // namespace ofp

#endif  // OFP_OXMIDRANGE_H_

// Copyright (c) 2015-2018 William W. Fisher (at gmail dot com)
// This file is distributed under the MIT License.

#ifndef OFP_BUCKETRANGE_H_
#define OFP_BUCKETRANGE_H_

#include "ofp/bucket.h"
#include "ofp/protocolrange.h"

namespace ofp {

using BucketIterator = ProtocolIterator<Bucket, ProtocolIteratorType::Bucket>;
using BucketRange = ProtocolRange<BucketIterator>;

}  // namespace ofp

#endif  // OFP_BUCKETRANGE_H_

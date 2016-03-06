// Copyright (c) 2015-2016 William W. Fisher (at gmail dot com)
// This file is distributed under the MIT License.

#include "ofp/mpgroupstats.h"
#include "ofp/validation.h"
#include "ofp/writable.h"

using namespace ofp;

PacketCounterRange MPGroupStats::bucketStats() const {
  assert(length_ >= sizeof(MPGroupStats));
  return ByteRange{BytePtr(this) + sizeof(MPGroupStats),
                   length_ - sizeof(MPGroupStats)};
}

bool MPGroupStats::validateInput(Validation *context) const {
  if (!context->validateAlignedLength(length_, sizeof(MPGroupStats))) {
    return false;
  }

  if (!bucketStats().validateInput(context)) {
    return false;
  }

  return true;
}

void MPGroupStatsBuilder::write(Writable *channel) {
  msg_.length_ = UInt16_narrow_cast(sizeof(msg_) + bucketStats_.size());

  channel->write(&msg_, sizeof(msg_));
  channel->write(bucketStats_.data(), bucketStats_.size());
  channel->flush();
}

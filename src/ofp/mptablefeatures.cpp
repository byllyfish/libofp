// Copyright 2014-present Bill Fisher. All rights reserved.

#include "ofp/mptablefeatures.h"
#include "ofp/writable.h"
#include "ofp/validation.h"

using namespace ofp;

PropertyRange MPTableFeatures::properties() const {
  // N.B. This length_ is padded out; just in case.
  return SafeByteRange(this, PadLength(length_), sizeof(MPTableFeatures));
}

bool MPTableFeatures::validateInput(Validation *context) const {
  size_t lengthRemaining = context->lengthRemaining();

  if (lengthRemaining < sizeof(MPTableFeatures)) {
    return false;
  }

  size_t len = length_;
  if (len < sizeof(MPTableFeatures)) {
    return false;
  }

  if ((len % 8) != 0) {
    log::info("Unpadded length in MPTableFeatures", len);
    len = PadLength(len);
  }

  if (len > lengthRemaining) {
    return false;
  }

  PropertyRange props = properties();
  if (!props.validateInput(context)) {
    return false;
  }

  if (!TableFeaturePropertyValidator::validateInput(props, context)) {
    return false;
  }

  return true;
}

void MPTableFeaturesBuilder::write(Writable *channel) {
  msg_.length_ = UInt16_narrow_cast(sizeof(msg_) + properties_.size());
  channel->write(&msg_, sizeof(msg_));
  channel->write(properties_.data(), properties_.size());
  channel->flush();
}

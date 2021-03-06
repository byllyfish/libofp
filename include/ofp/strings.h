// Copyright (c) 2015-2018 William W. Fisher (at gmail dot com)
// This file is distributed under the MIT License.

#ifndef OFP_STRINGS_H_
#define OFP_STRINGS_H_

#include "ofp/constants.h"
#include "ofp/smallcstring.h"

namespace ofp {

// String types for port and table names.

using PortNameStr = SmallCString<16>;
using TableNameStr = SmallCString<32>;

// String types used in MPDesc.

using DescriptionStr = SmallCString<256>;
using SerialNumberStr = SmallCString<32>;

// Compile time assertions.

static_assert(IsStandardLayout<PortNameStr>(), "Expected standard layout.");
static_assert(IsTriviallyCopyable<PortNameStr>(),
              "Expected trivially copyable.");

static_assert(IsStandardLayout<TableNameStr>(), "Expected standard layout.");
static_assert(IsTriviallyCopyable<TableNameStr>(),
              "Expected trivially copyable.");

static_assert(IsStandardLayout<DescriptionStr>(), "Expected standard layout.");
static_assert(IsTriviallyCopyable<DescriptionStr>(),
              "Expected trivially copyable.");

static_assert(IsStandardLayout<SerialNumberStr>(), "Expected standard layout.");
static_assert(IsTriviallyCopyable<SerialNumberStr>(),
              "Expected trivially copyable.");

}  // namespace ofp

#endif  // OFP_STRINGS_H_

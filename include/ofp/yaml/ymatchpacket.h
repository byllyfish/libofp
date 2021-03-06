// Copyright (c) 2015-2018 William W. Fisher (at gmail dot com)
// This file is distributed under the MIT License.

#ifndef OFP_YAML_YMATCHPACKET_H_
#define OFP_YAML_YMATCHPACKET_H_

#include "ofp/matchpacket.h"
#include "ofp/yaml/ymatch.h"

namespace llvm {
namespace yaml {

template <>
struct SequenceTraits<ofp::MatchPacket> {
  using iterator = ofp::OXMIterator;

  static iterator begin(IO &io, ofp::MatchPacket &match) {
    return match.begin();
  }

  static iterator end(IO &io, ofp::MatchPacket &match) { return match.end(); }

  static void next(iterator &iter, iterator iterEnd) { ++iter; }
};

}  // namespace yaml
}  // namespace llvm

#endif  // OFP_YAML_YMATCHPACKET_H_

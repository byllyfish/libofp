// Copyright 2014-present Bill Fisher. All rights reserved.

#ifndef OFP_YAML_YMATCHPACKET_H_
#define OFP_YAML_YMATCHPACKET_H_

#include "ofp/yaml/ymatch.h"
#include "ofp/matchpacket.h"

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
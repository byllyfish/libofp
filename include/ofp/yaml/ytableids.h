// Copyright (c) 2015-2018 William W. Fisher (at gmail dot com)
// This file is distributed under the MIT License.

#ifndef OFP_YAML_YTABLEIDS_H_
#define OFP_YAML_YTABLEIDS_H_

#include "ofp/tableidlist.h"

namespace ofp {
namespace detail {

struct TableIDInserter {};

}  // namespace detail
}  // namespace ofp

namespace llvm {
namespace yaml {

template <>
struct ScalarTraits<ofp::detail::TableIDInserter> {
  static void output(const ofp::detail::TableIDInserter &value, void *ctxt,
                     llvm::raw_ostream &out) {}

  static StringRef input(StringRef scalar, void *ctxt,
                         ofp::detail::TableIDInserter &value) {
    ofp::TableIDList &list = Ref_cast<ofp::TableIDList>(value);

    ofp::UInt8 id;
    StringRef result = ScalarTraits<ofp::UInt8>::input(scalar, ctxt, id);
    if (result.empty()) {
      list.add(id);
    }
    return result;
  }

  static QuotingType mustQuote(StringRef) { return QuotingType::None; }
};

template <>
struct SequenceTraits<ofp::TableIDRange> {
  // Use nthItem; It's fast enough.

  static size_t size(IO &io, ofp::TableIDRange &range) { return range.size(); }

  static ofp::UInt8 &element(IO &io, ofp::TableIDRange &range, size_t index) {
    const ofp::UInt8 *iter = range.nthItem(index);
    return ofp::RemoveConst_cast(*iter);
  }

  static const bool flow = true;
};

template <>
struct SequenceTraits<ofp::TableIDList> {
  static size_t size(IO &io, ofp::TableIDList &list) { return 0; }

  static ofp::detail::TableIDInserter &element(IO &io, ofp::TableIDList &list,
                                               size_t index) {
    return Ref_cast<ofp::detail::TableIDInserter>(list);
  }
};

}  // namespace yaml
}  // namespace llvm

#endif  // OFP_YAML_YTABLEIDS_H_

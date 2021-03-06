// Copyright (c) 2015-2018 William W. Fisher (at gmail dot com)
// This file is distributed under the MIT License.

#ifndef OFP_YAML_YLLDPVALUE_H_
#define OFP_YAML_YLLDPVALUE_H_

#include "ofp/lldpvalue.h"

namespace llvm {
namespace yaml {

template <ofp::LLDPType Type>
struct ScalarTraits<ofp::LLDPValue<Type>> {
  static void output(const ofp::LLDPValue<Type> &value, void *ctxt,
                     llvm::raw_ostream &out) {
    out << value;
  }

  static StringRef input(StringRef scalar, void *ctxt,
                         ofp::LLDPValue<Type> &value) {
    if (!value.parse(scalar))
      return "Invalid LLDP Value.";
    return "";
  }

  static QuotingType mustQuote(StringRef) { return QuotingType::Single; }
};

}  // namespace yaml
}  // namespace llvm

#endif  // OFP_YAML_YLLDPVALUE_H_

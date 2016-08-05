#ifndef OFP_YAML_YACTIONFULLTYPE_H_
#define OFP_YAML_YACTIONFULLTYPE_H_

#include "ofp/actiontype.h"

namespace llvm {
namespace yaml {

template <>
struct ScalarTraits<ofp::ActionFullType> {
  static void output(const ofp::ActionFullType &value, void *ctxt,
                     llvm::raw_ostream &out) {
    auto info = value.lookupInfo();
    if (info) {
      out << info->name;
    } else if (value.experimenter() == 0) {
      // Format as:  `action`
      out << llvm::format("0x%04X", value.enumType());
    } else {
      // Format as:  `action.experimenter`
      out << llvm::format("0x%04X.0x%08X", value.enumType(),
                          value.experimenter());
    }
  }

  static StringRef input(StringRef scalar, void *ctxt,
                         ofp::ActionFullType &value) {
    if (value.parse(scalar)) {
      return "";
    }

    // Check for format `action.experimenter`
    auto pair = scalar.split('.');
    if (!pair.second.empty()) {
      ofp::UInt16 action;
      ofp::UInt32 experimenter;
      if (ofp::yaml::ParseUnsignedInteger(pair.first, &action) &&
          ofp::yaml::ParseUnsignedInteger(pair.second, &experimenter)) {
        value.setNative(action, experimenter);
        return "";
      }
    }

    // Check for format `action` only.
    ofp::UInt16 num;
    if (ofp::yaml::ParseUnsignedInteger(scalar, &num)) {
      value.setNative(num, 0);
      return "";
    }

    return "Invalid action type.";
  }

  static bool mustQuote(StringRef) { return false; }
};

}  // namespace yaml
}  // namespace llvm

#endif // OFP_YAML_YACTIONFULLTYPE_H_
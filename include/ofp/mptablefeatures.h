#ifndef OFP_MPTABLEFEATURES_H_
#define OFP_MPTABLEFEATURES_H_

#include "ofp/propertylist.h"
#include "ofp/strings.h"

namespace ofp {

class Writable;

class MPTableFeatures {
 public:
  enum { MPVariableSizeOffset = 0 };

  UInt8 tableId() const { return tableId_; }
  const TableNameStr &name() const { return name_; }
  UInt64 metadataMatch() const { return metadataMatch_; }
  UInt64 metadataWrite() const { return metadataWrite_; }
  UInt32 config() const { return config_; }
  UInt32 maxEntries() const { return maxEntries_; }

  PropertyRange properties() const;

  bool validateInput(Validation *context) const;

 private:
  Big16 length_;
  Big8 tableId_;
  Padding<5> pad_;
  TableNameStr name_;
  Big64 metadataMatch_;
  Big64 metadataWrite_;
  Big32 config_;
  Big32 maxEntries_;

  friend class MPTableFeaturesBuilder;
  template <class T>
  friend struct llvm::yaml::MappingTraits;
};

static_assert(sizeof(MPTableFeatures) == 64, "Unexpected size.");
static_assert(IsStandardLayout<MPTableFeatures>(), "Expected standard layout.");

class MPTableFeaturesBuilder {
 public:
  void setTableId(UInt8 tableId) { msg_.tableId_ = tableId; }
  void setName(TableNameStr name) { msg_.name_ = name; }
  void setMetadataMatch(UInt64 metadataMatch) {
    msg_.metadataMatch_ = metadataMatch;
  }
  void setMetadataWrite(UInt64 metadataWrite) {
    msg_.metadataWrite_ = metadataWrite;
  }
  void setConfig(UInt32 config) { msg_.config_ = config; }
  void setMaxEntries(UInt32 maxEntries) { msg_.maxEntries_ = maxEntries; }

  void setProperties(const PropertyList &properties) {
    properties_ = properties;
  }

  void write(Writable *channel);
  void reset() { properties_.clear(); }

 private:
  MPTableFeatures msg_;
  PropertyList properties_;

  template <class T>
  friend struct llvm::yaml::MappingTraits;
};

}  // namespace ofp

#endif  // OFP_MPTABLEFEATURES_H_

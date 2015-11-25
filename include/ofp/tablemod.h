// Copyright 2014-present Bill Fisher. All rights reserved.

#ifndef OFP_TABLEMOD_H_
#define OFP_TABLEMOD_H_

#include "ofp/protocolmsg.h"
#include "ofp/padding.h"
#include "ofp/tablenumber.h"
#include "ofp/propertylist.h"

namespace ofp {

class TableMod : public ProtocolMsg<TableMod, OFPT_TABLE_MOD, 16> {
 public:
  TableNumber tableId() const { return tableId_; }
  OFPTableConfigFlags config() const { return config_; }

  PropertyRange properties() const;

  bool validateInput(Validation *context) const;

 private:
  Header header_;
  TableNumber tableId_;
  Padding<3> pad_;
  Big<OFPTableConfigFlags> config_;

  // Only TableModBuilder can create an instance.
  TableMod() : header_{type()} {}

  friend class TableModBuilder;
  template <class T>
  friend struct llvm::yaml::MappingTraits;
};

static_assert(sizeof(TableMod) == 16, "Unexpected size.");
static_assert(IsStandardLayout<TableMod>(), "Expected standard layout.");
static_assert(IsTriviallyCopyable<TableMod>(), "Expected trivially copyable.");

class TableModBuilder {
 public:
  TableModBuilder() = default;
  explicit TableModBuilder(const TableMod *msg);

  void setTableId(TableNumber tableId) { msg_.tableId_ = tableId; }
  void setConfig(OFPTableConfigFlags config) { msg_.config_ = config; }

  void setProperties(const PropertyList &properties) {
    properties_ = properties;
  }
  
  UInt32 send(Writable *channel);

 private:
  TableMod msg_;
  PropertyList properties_;

  template <class T>
  friend struct llvm::yaml::MappingTraits;
};

}  // namespace ofp

#endif  // OFP_TABLEMOD_H_

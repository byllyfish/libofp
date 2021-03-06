// Copyright (c) 2015-2018 William W. Fisher (at gmail dot com)
// This file is distributed under the MIT License.

#ifndef OFP_ROLESTATUS_H_
#define OFP_ROLESTATUS_H_

#include "ofp/padding.h"
#include "ofp/propertylist.h"
#include "ofp/protocolmsg.h"

namespace ofp {

class RoleStatus : public ProtocolMsg<RoleStatus, OFPT_ROLE_STATUS, 24, 65528> {
 public:
  OFPControllerRole role() const { return role_; }
  OFPRoleStatusReason reason() const { return reason_; }
  UInt64 generationId() const { return generationId_; }

  PropertyRange properties() const;

  bool validateInput(Validation *context) const;

 private:
  Header header_;
  Big<OFPControllerRole> role_;
  Big<OFPRoleStatusReason> reason_;
  Padding<3> pad_;
  Big64 generationId_;

  // Only RoleStatusBuilder can construct an instance.
  RoleStatus() : header_{type()} {}

  friend class RoleStatusBuilder;
  template <class T>
  friend struct llvm::yaml::MappingTraits;
};

static_assert(sizeof(RoleStatus) == 24, "Unexpected size.");
static_assert(IsStandardLayout<RoleStatus>(), "Expected standard layout.");
static_assert(IsTriviallyCopyable<RoleStatus>(),
              "Expected trivially copyable.");

class RoleStatusBuilder {
 public:
  void setRole(OFPControllerRole role) { msg_.role_ = role; }
  void setReason(OFPRoleStatusReason reason) { msg_.reason_ = reason; }
  void setGenerationId(UInt64 generationId) {
    msg_.generationId_ = generationId;
  }
  void setProperties(const PropertyList &properties);

  UInt32 send(Writable *channel);

 private:
  RoleStatus msg_;
  PropertyList properties_;

  template <class T>
  friend struct llvm::yaml::MappingTraits;
};

}  // namespace ofp

#endif  // OFP_ROLESTATUS_H_

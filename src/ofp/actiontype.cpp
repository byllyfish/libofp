// Copyright 2014-present Bill Fisher. All rights reserved.

#include "ofp/actiontype.h"
#include "ofp/actions.h"

using namespace ofp;

static const ActionTypeInfo sActionInfo[] = {
    {AT_OUTPUT::type(), "OUTPUT"},
    {AT_COPY_TTL_OUT::type(), "COPY_TTL_OUT"},
    {AT_COPY_TTL_IN::type(), "COPY_TTL_IN"},
    {AT_SET_MPLS_TTL::type(), "SET_MPLS_TTL"},
    {AT_DEC_MPLS_TTL::type(), "DEC_MPLS_TTL"},
    {AT_PUSH_VLAN::type(), "PUSH_VLAN"},
    {AT_POP_VLAN::type(), "POP_VLAN"},
    {AT_PUSH_MPLS::type(), "PUSH_MPLS"},
    {AT_POP_MPLS::type(), "POP_MPLS"},
    {AT_SET_QUEUE::type(), "SET_QUEUE"},
    {AT_GROUP::type(), "GROUP"},
    {AT_SET_NW_TTL::type(), "SET_NW_TTL"},
    {AT_DEC_NW_TTL::type(), "DEC_NW_TTL"},
    {AT_PUSH_PBB::type(), "PUSH_PBB"},
    {AT_POP_PBB::type(), "POP_PBB"},
    {deprecated::AT_ENQUEUE_V1::type(), "ENQUEUE"},
    {ActionType{OFPAT_SET_FIELD, 0}, "SET_FIELD"},
    {ActionType{OFPAT_EXPERIMENTER, 0}, "EXPERIMENTER"}};

bool ActionType::parse(const std::string &s) {
  for (unsigned i = 0; i < ArrayLength(sActionInfo); ++i) {
    if (s == sActionInfo[i].name) {
      value32_ = sActionInfo[i].type.value32_;
      return true;
    }
  }
  return false;
}

const ActionTypeInfo *ActionType::lookupInfo() const {
  for (unsigned i = 0; i < ArrayLength(sActionInfo); ++i) {
    if (value32_ == sActionInfo[i].type) {
      return &sActionInfo[i];
    }
  }

  // Ignore length when checking for SET_FIELD or EXPERIMENTER actions.
  const UInt32 mask = BigEndianToNative(0xFFFF0000);
  UInt32 desiredValue = (value32_ & mask);
  for (unsigned i = ArrayLength(sActionInfo) - 2; i < ArrayLength(sActionInfo); ++i) {
    if (desiredValue == sActionInfo[i].type) {
      return &sActionInfo[i];
    }
  }  

  return nullptr;
}

const ActionTypeInfo *ActionType::lookupInfo_IgnoreLength() const {
  const UInt32 mask = BigEndianToNative(0xFFFF0000);
  UInt32 desiredValue = (value32_ & mask);
  for (unsigned i = 0; i < ArrayLength(sActionInfo); ++i) {
    if (desiredValue == (sActionInfo[i].type & mask)) {
      return &sActionInfo[i];
    }
  }

  return nullptr;
}

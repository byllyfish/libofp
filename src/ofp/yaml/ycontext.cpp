// Copyright 2014-present Bill Fisher. All rights reserved.

#include "ofp/yaml/yllvm.h"
#include "ofp/yaml/ycontext.h"

using ofp::yaml::detail::YamlContext;

ofp::yaml::Encoder *ofp::yaml::GetEncoderFromContext(llvm::yaml::IO &io) {
  YamlContext *ctxt = reinterpret_cast<YamlContext *>(io.getContext());
  if (ctxt) {
    assert(ctxt->validate());
    return ctxt->encoder;
  }
  return nullptr;
}

ofp::yaml::Decoder *ofp::yaml::GetDecoderFromContext(llvm::yaml::IO &io) {
  YamlContext *ctxt = reinterpret_cast<YamlContext *>(io.getContext());
  if (ctxt) {
    assert(ctxt->validate());
    return ctxt->decoder;
  }
  return nullptr;
}

ofp::UInt8 ofp::yaml::GetVersionFromContext(llvm::yaml::IO &io) {
  YamlContext *ctxt = reinterpret_cast<YamlContext *>(io.getContext());
  if (ctxt) {
    assert(ctxt->validate());
    return ctxt->version;
  }
  return 0;
}

bool ofp::yaml::GetIncludePktMatchFromContext(llvm::yaml::IO &io) {
  YamlContext *ctxt = reinterpret_cast<YamlContext *>(io.getContext());
  if (ctxt) {
    assert(ctxt->validate());
    return ctxt->pktMatch;
  }
  return false;
}

ofp::yaml::Encoder *ofp::yaml::detail::YamlContext::GetEncoder(void *context) {
  YamlContext *ctxt = reinterpret_cast<YamlContext *>(context);
  if (ctxt) {
    assert(ctxt->validate());
    return ctxt->encoder;
  }
  return nullptr;
}
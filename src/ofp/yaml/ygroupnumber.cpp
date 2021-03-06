// Copyright (c) 2015-2018 William W. Fisher (at gmail dot com)
// This file is distributed under the MIT License.

#include "ofp/yaml/ygroupnumber.h"

using namespace ofp;
using ofp::yaml::EnumConverterSparse;

#define OFP_NAME(s) \
  { OFPG_##s, #s }

OFP_BEGIN_IGNORE_GLOBAL_CONSTRUCTOR

static const std::pair<OFPGroupNo, llvm::StringRef> sGroupNumbers[] = {
    OFP_NAME(ALL),
    OFP_NAME(ANY),
};

const EnumConverterSparse<OFPGroupNo>
    llvm::yaml::ScalarTraits<ofp::GroupNumber>::converter{sGroupNumbers};

OFP_END_IGNORE_GLOBAL_CONSTRUCTOR

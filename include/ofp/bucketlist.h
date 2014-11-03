//  ===== ---- ofp/bucketlist.h ----------------------------*- C++ -*- =====  //
//
//  Copyright (c) 2013 William W. Fisher
//
//  Licensed under the Apache License, Version 2.0 (the "License");
//  you may not use this file except in compliance with the License.
//  You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
//  Unless required by applicable law or agreed to in writing, software
//  distributed under the License is distributed on an "AS IS" BASIS,
//  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//  See the License for the specific language governing permissions and
//  limitations under the License.
//
//  ===== ------------------------------------------------------------ =====  //
/// \file
/// \brief Defines the BucketList class.
//  ===== ------------------------------------------------------------ =====  //

#ifndef OFP_BUCKETLIST_H_
#define OFP_BUCKETLIST_H_

#include "ofp/protocollist.h"
#include "ofp/bucketrange.h"

namespace ofp {

class BucketList : public ProtocolList<BucketRange> {
  using Inherited = ProtocolList<BucketRange>;

 public:
  using Inherited::Inherited;

  void add(const BucketBuilder &bucket);
};

}  // namespace ofp

#endif  // OFP_BUCKETLIST_H_

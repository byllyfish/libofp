// Copyright (c) 2015-2018 William W. Fisher (at gmail dot com)
// This file is distributed under the MIT License.

#ifndef OFP_INSTRUCTIONLIST_H_
#define OFP_INSTRUCTIONLIST_H_

#include "ofp/bytelist.h"
#include "ofp/instructioniterator.h"
#include "ofp/instructionrange.h"
#include "ofp/instructions.h"
#include "ofp/protocollist.h"

namespace ofp {

class InstructionList : public ProtocolList<InstructionRange> {
  using Inherited = ProtocolList<InstructionRange>;

 public:
  using Inherited::Inherited;

  template <class Type>
  void add(const Type &instruction);
};

template <class Type>
inline void InstructionList::add(const Type &instruction) {
  static_assert(Type::type().enumType() != 0, "Type is not an instruction?");
  buf_.add(&instruction, sizeof(instruction));
}

template <>
inline void InstructionList::add(const IT_WRITE_ACTIONS &instruction) {
  buf_.add(&instruction, IT_WRITE_ACTIONS::HeaderSize);
  buf_.add(instruction.data(), instruction.size());
}

template <>
inline void InstructionList::add(const IT_APPLY_ACTIONS &instruction) {
  buf_.add(&instruction, IT_APPLY_ACTIONS::HeaderSize);
  buf_.add(instruction.data(), instruction.size());
}

template <>
inline void InstructionList::add(const IT_EXPERIMENTER &instruction) {
  buf_.add(&instruction, IT_EXPERIMENTER::HeaderSize);
  buf_.add(instruction.data(), instruction.size());
}

}  // namespace ofp

#endif  // OFP_INSTRUCTIONLIST_H_

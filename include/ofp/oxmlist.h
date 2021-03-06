// Copyright (c) 2015-2018 William W. Fisher (at gmail dot com)
// This file is distributed under the MIT License.

#ifndef OFP_OXMLIST_H_
#define OFP_OXMLIST_H_

#include "ofp/bytelist.h"
#include "ofp/oxmrange.h"

namespace ofp {

// OXMList is an ordered sequence of values stored (without padding) in a
// memory buffer. It allows multiple values for the same key (used for prereqs).

class OXMList {
 public:
  OXMList() = default;
  /* implicit NOLINT */ OXMList(const OXMRange &range);

  OXMIterator begin() const { return OXMIterator(buf_.begin()); }
  OXMIterator end() const { return OXMIterator(buf_.end()); }

  const UInt8 *data() const { return buf_.begin(); }
  size_t size() const { return buf_.size(); }

  OXMRange toRange() const { return OXMRange{buf_.data(), buf_.size()}; }

  // Add regular value (non-experimenter).
  template <class ValueType>
  EnableIf<!detail::has_ExperimenterTraits<ValueType>::value, void> add(
      ValueType value) {
    static_assert(sizeof(value) < 256, "oxm_length must be <= 255.");
    add(ValueType::type(), &value, sizeof(value));
  }

  // Add experimenter value.
  template <class ValueType>
  EnableIf<detail::has_ExperimenterTraits<ValueType>::value, void> add(
      ValueType value) {
    static_assert(sizeof(value) < 256, "oxm_length must be <= 255.");
    addExperimenter(ValueType::type(), ValueType::experimenter(), &value,
                    sizeof(value));
  }

  // Add regular value and mask (non-experimenter).
  template <class ValueType>
  EnableIf<!detail::has_ExperimenterTraits<ValueType>::value, void> add(
      ValueType value, ValueType mask) {
    static_assert(sizeof(value) < 128, "oxm_length must be <= 255.");
    add(ValueType::type().withMask(), &value, &mask, sizeof(value));
  }

  // Add experimenter value and mask.
  template <class ValueType>
  EnableIf<detail::has_ExperimenterTraits<ValueType>::value, void> add(
      ValueType value, ValueType mask) {
    static_assert(sizeof(value) < 128, "oxm_length must be <= 255.");
    addExperimenter(ValueType::type().withMask(), ValueType::experimenter(),
                    &value, &mask, sizeof(value));
  }

  template <class ValueType>
  void replace(ValueType value);

  void add(OXMType type, const void *data, size_t len) {
    assert(type.length() == len);
    buf_.add(&type, sizeof(type));
    buf_.add(data, len);
  }

  void addExperimenter(OXMType type, Big32 experimenter, const void *data,
                       size_t len) {
    assert(type.length() == len + 4);
    buf_.add(&type, sizeof(type));
    buf_.add(&experimenter, sizeof(experimenter));
    buf_.add(data, len);
  }

  void add(OXMType type, const void *data, const void *mask, size_t len) {
    assert(type.length() == 2 * len);
    buf_.add(&type, sizeof(type));
    buf_.addMasked(data, mask, len);
    buf_.add(mask, len);
  }

  void addExperimenter(OXMType type, Big32 experimenter, const void *data,
                       const void *mask, size_t len) {
    log_debug("addExperimenter", type.length(), len);
    assert(type.length() == sizeof(experimenter) + 2 * len);
    buf_.add(&type, sizeof(type));
    buf_.add(&experimenter, sizeof(experimenter));
    buf_.addMasked(data, mask, len);
    buf_.add(mask, len);
  }

  void addOrdered(OXMType type, const void *data, size_t len);

  void add(OXMIterator iter);
  void addSignal(OXMType signal);
  void insertSignal(OXMIterator pos, OXMType signal);
  void pad8(unsigned offset);

  void clear() { buf_.clear(); }

  OXMIterator replace(OXMIterator pos, OXMIterator end, OXMType type,
                      const void *data, size_t len);

  bool operator==(const OXMList &rhs) const { return buf_ == rhs.buf_; }
  bool operator!=(const OXMList &rhs) const { return !(*this == rhs); }

 private:
  ByteList buf_;

  void insert(OXMIterator pos, const void *data, size_t len);

  OXMIterator findValue(OXMType type) const;
  OXMIterator findOrderedPos(OXMType type) const;
  void replaceValue(OXMIterator pos, const void *data, size_t len);
};

template <class ValueType>
inline void OXMList::replace(ValueType value) {
  static_assert(sizeof(value) < 256, "oxm_length must be <= 255.");

  OXMIterator iter = this->findValue(ValueType::type());
  if (iter != end()) {
    this->replaceValue(iter, &value, sizeof(value));
  } else {
    add(ValueType::type(), &value, sizeof(value));
  }
}

inline void OXMList::add(OXMIterator iter) {
  buf_.add(iter.data(), iter.size());
}

inline void OXMList::addSignal(OXMType signal) {
  buf_.add(&signal, sizeof(signal));
}

inline void OXMList::insertSignal(OXMIterator pos, OXMType signal) {
  insert(pos, &signal, sizeof(signal));
}

inline void OXMList::pad8(unsigned n) {
  size_t len = PadLength(n + size()) - (n + size());
  if (len > 0) {
    assert(len < 8);
    Padding<8> padding;
    buf_.add(&padding, len);
  }
}

}  // namespace ofp

#endif  // OFP_OXMLIST_H_

//  ===== ---- ofp/bytelist.h ------------------------------*- C++ -*- =====  //
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
/// \brief Defines the ByteList class.
//  ===== ------------------------------------------------------------ =====  //

#ifndef OFP_BYTELIST_H
#define OFP_BYTELIST_H

#include "ofp/byterange.h"
#include <vector>

namespace ofp { // <namespace ofp>

/// \brief Concrete class representing a mutable byte buffer.
///
/// This class manages its own memory. The memory underlying a ByteList is
/// relocatable; methods that change the size of the buffer can cause pointers
/// into the ByteList to become invalid.
///
/// When you copy or assign a ByteList, it produces a deep copy.
///
/// @see ByteData
/// @see ByteRange
class ByteList {
public:
  /// \brief Construct an empty byte buffer.
  ByteList() = default;

  /// \brief Construct byte buffer copying data in range.
  ByteList(const ByteRange &range);

  /// \brief Construct byte buffer copying the specified data.
  ByteList(const void *data, size_t length) { add(data, length); }

  /// \returns pointer to beginning of constant byte buffer.
  const UInt8 *begin() const { return &buf_[0]; }

  /// \returns pointer to end of constant byte buffer (i.e. one past last byte).
  const UInt8 *end() const { return &buf_[buf_.size()]; }

  /// \returns pointer to beginning of constant byte buffer.
  const UInt8 *data() const { return &buf_[0]; }

  /// \returns pointer to beginning of mutable byte buffer.
  UInt8 *mutableData() { return &buf_[0]; }

  /// \returns size of buffer in bytes.
  size_t size() const { return buf_.size(); }

  /// \returns offset of pointer relative to start of the byte buffer.
  ptrdiff_t offset(const UInt8 *pos) const {
    assertInRange(pos);
    return pos - data();
  }

  /// \brief Set contents of the entire byte buffer.
  void set(const void *data, size_t length) {
    assert(data != nullptr || length == 0);
    clear();
    add(data, length);
  }

  /// \brief Append data to the end of the byte buffer.
  void add(const void *data, size_t length) {
    assert(data != nullptr || length == 0);
    auto bp = BytePtr(data);
    buf_.insert(buf_.end(), bp, bp + length);
  }

  /// \brief Insert data at the specified position in the byte buffer.
  void insert(const UInt8 *pos, const void *data, size_t length) {
    assertInRange(pos);
    assert(data != nullptr || length == 0);
    auto bp = BytePtr(data);
    buf_.insert(buf_.begin() + offset(pos), bp, bp + length);
  }

  /// \brief Replace existing data in the byte buffer with new values.
  void replace(const UInt8 *pos, const UInt8 *posEnd, const void *data,
               size_t length) {
    assert(data != nullptr || length == 0);
    auto idx = offset(pos);
    replaceUninitialized(pos, posEnd, length);
    // N.B. Memory might have moved.
    auto bp = BytePtr(data);
    std::copy(bp, bp + length, buf_.begin() + idx);
  }

  /// \brief Append uninitialized bytes to the end of the byte buffer.
  void addUninitialized(size_t length) { buf_.insert(buf_.end(), length, 0); }

  /// \brief Insert uninitialized bytes at the specified position in the byte
  /// buffer.
  void insertUninitialized(const UInt8 *pos, size_t length) {
    assertInRange(pos);
    buf_.insert(buf_.begin() + offset(pos), length, 0);
  }

  /// \brief Replace existing data in the byte buffer with uninitialized values.
  void replaceUninitialized(const UInt8 *pos, const UInt8 *posEnd,
                            size_t length) {
    assert(pos <= posEnd);
    assertInRange(pos);
    assertInRange(posEnd);

    size_t oldlen = Unsigned_cast(posEnd - pos);
    if (length > oldlen) {
      buf_.insert(buf_.begin() + offset(posEnd), length - oldlen, 0);
    } else if (length < oldlen) {
      auto iter = buf_.begin() + offset(pos);
      buf_.erase(iter, iter + Signed_cast(oldlen - length));
    }
  }

  /// \brief Remove specified range from the byte buffer.
  void remove(const UInt8 *pos, size_t length) {
    assertInRange(pos);
    auto iter = buf_.begin() + offset(pos);
    buf_.erase(iter, iter + Signed_cast(length));
  }

  /// \brief Resize buffer to specified length.
  void resize(size_t length) { buf_.resize(length); }

  /// \brief Clear contents of the buffer.
  void clear() { buf_.clear(); }

  bool operator==(const ByteList &rhs) const { return buf_ == rhs.buf_; }
  bool operator!=(const ByteList &rhs) const { return !operator==(rhs); }

  ByteRange toRange() const;
  void operator=(const ByteRange &range);

private:
  std::vector<UInt8> buf_;

  /// \brief Helper function to verify pointer is valid for byte buffer.
  void assertInRange(const UInt8 *pos) const {
    assert((pos != nullptr) && "position is null");
    assert((pos >= begin() && pos <= end()) && "position out of range");
  }
};

/// \brief Write buffer to stream in hexadecimal format.
std::ostream &operator<<(std::ostream &os, const ByteList &value);

inline std::ostream &operator<<(std::ostream &os, const ByteList &value) {
  return os << RawDataToHex(value.data(), value.size(), ' ', 2);
}

} // </namespace ofp>

#endif // OFP_BYTELIST_H

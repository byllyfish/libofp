//  ===== ---- ofp/types.h ---------------------------------*- C++ -*- =====  //
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
/// \brief Define globally useful constants, types and functions.
//  ===== ------------------------------------------------------------ =====  //

#ifndef OFP_TYPES_H
#define OFP_TYPES_H

#include "ofp/config.h"
#include <cstddef>     // for std::size_t, etc.
#include <cstdint>     // for std::uint8_t, etc.
#include <cstring>     // for std::strlen, std::memcpy, etc.
#include <cstdlib>     // for std::malloc, etc.
#include <type_traits> // for std::make_unsigned<T>, etc.
#include <string>      // for std::string
#include <cassert>     // for assert macro
#include <ostream>     // for std::ostream (used for now)
#include <chrono>

#if defined(__clang__)
#define OFP_BEGIN_IGNORE_PADDING                                               \
  _Pragma("clang diagnostic push")                                             \
      _Pragma("clang diagnostic ignored \"-Wpadded\"")
#define OFP_END_IGNORE_PADDING _Pragma("clang diagnostic pop")
#else
#define OFP_BEGIN_IGNORE_PADDING
#define OFP_END_IGNORE_PADDING
#endif

#if defined(__clang__)
#define OFP_ALIGNAS(x) alignas(x)
#else
#define OFP_ALIGNAS(x) __attribute__((aligned(x)))
#endif

namespace ofp { // <namespace ofp>

// Metaprogramming Utilities

template <bool B, class T>
using EnableIf = typename std::enable_if<B, T>::type;

template <bool B, class T, class F>
using Conditional = typename std::conditional<B, T, F>::type;

template <class T>
using MakeSigned = typename std::make_signed<T>::type;

template <class T>
using MakeUnsigned = typename std::make_unsigned<T>::type;

// Be careful when using UInt8 and UInt16. By default, C++ will promote these
// to an `int` for bitwise and arithmetic operations, so you need to be
// alert for sign extension. Prefer operations against `unsigned int` types.
// See 10.5.1 in TC++PL, 4th ed.

using UInt8 = std::uint8_t;
using UInt16 = std::uint16_t;
using UInt32 = std::uint32_t;
using UInt64 = std::uint64_t;

using Milliseconds = std::chrono::milliseconds;

// Literal type `ms` is not supported until C++14. Use _ms for now.

constexpr Milliseconds operator"" _ms(unsigned long long msec) {
  return Milliseconds{msec};
}

template <class T>
constexpr MakeSigned<T> Signed_cast(T value) {
  return static_cast<MakeSigned<T>>(value);
}

template <class T>
constexpr MakeUnsigned<T> Unsigned_cast(T value) {
  return static_cast<MakeUnsigned<T>>(value);
}

template <class T>
constexpr UInt8 UInt8_cast(T value) {
  static_assert(sizeof(T) == sizeof(UInt8), "Use UInt8_narrow_cast().");
  return static_cast<UInt8>(Unsigned_cast(value));
}

template <class T>
constexpr UInt16 UInt16_cast(T value) {
  static_assert(sizeof(T) <= sizeof(UInt16), "Use UInt16_narrow_cast().");
  return static_cast<UInt16>(Unsigned_cast(value));
}

template <class T>
constexpr UInt32 UInt32_cast(T value) {
  static_assert(sizeof(T) <= sizeof(UInt32), "Use UInt32_narrow_cast().");
  return static_cast<UInt32>(Unsigned_cast(value));
}

template <class T>
constexpr UInt64 UInt64_cast(T value) {
  static_assert(sizeof(T) <= sizeof(UInt64), "No UInt64 narrow casts.");
  return static_cast<UInt64>(Unsigned_cast(value));
}

template <class T>
constexpr UInt8 UInt8_narrow_cast(T value) {
  static_assert(sizeof(T) != sizeof(UInt8), "Use UInt8_cast().");
  return static_cast<UInt8>(value);
}

template <class T>
constexpr UInt16 UInt16_narrow_cast(T value) {
  static_assert(sizeof(T) != sizeof(UInt16), "Use UInt16_cast().");
  return static_cast<UInt16>(value);
}

template <class T>
constexpr UInt16 UInt32_narrow_cast(T value) {
  static_assert(sizeof(T) != sizeof(UInt32), "Use UInt32_cast().");
  return static_cast<UInt32>(value);
}

/// Convenience function to compute the difference of two unsigned integers. It
/// prevents underflow or implicit type promotion. If the right argument is
/// greater than the left argument, this function returns zero. This function
/// requires that both arguments are exactly the same unsigned type.
///
/// \param  lhs left hand side.
/// \param  rhs right hand side.
/// \return difference between lhs and rhs or zero.
template <class T>
constexpr EnableIf<std::is_unsigned<T>::value, T> Unsigned_difference(T lhs,
                                                                      T rhs) {
  return lhs >= rhs ? lhs - rhs : 0;
}

/// \returns true if type is a literal type.
template <class T>
constexpr bool IsLiteralType() {
  return std::is_literal_type<T>::value;
}

/// \returns true if type has standard layout.
template <class T>
constexpr bool IsStandardLayout() {
  return std::is_standard_layout<T>::value;
}

/// \returns true if type is trivially copyable.
template <class T>
constexpr bool IsTriviallyCopyable() {
#if defined(__clang__)
  return std::is_trivially_copyable<T>::value;
#else
  // GCC 4.7.2 doesn't define std::is_trivially_copyable. We only use this
  // macro in static_asserts, so the easiest fix is to always return true.
  return true;
#endif
}

/// \returns true if type `From` can be implicitly converted to type `To`.
template <class From, class To>
constexpr bool IsConvertible() {
  return std::is_convertible<From,To>::value;
}

/// \returns number of elements in array.
template <class T, size_t N>
constexpr size_t ArrayLength(T (&)[N]) {
  return N;
}

/// \returns byte pointer for any memory address.
constexpr const UInt8 *BytePtr(const void *data) {
  return static_cast<const UInt8 *>(data);
}

/// Removes a `const` qualifier from a reference. This is necessary in rare
/// cases when interacting with important external API's that are less
/// `const-strict`. No other instances of const_cast should appear in the code.
template <class T>
T &RemoveConst_cast(const T &v) {
  return const_cast<T &>(v);
}

template <class T>
T *RemoveConst_cast(const T *v) {
  return const_cast<T *>(v);
}

/// Convert raw buffer to a hexadecimal string. The resulting string contains
/// only hexadecimal characters.
///
/// \param  data pointer to input buffer
/// \param  length size of input buffer
/// \return hexadecimal string
std::string RawDataToHex(const void *data, size_t length);

/// Convert raw buffer to a hexadecimal string.
///
/// The resulting string isformatted according to `delimiter` and `word`. The
/// delimiter specifies a character to insert between each run of hexadecimal
/// chars. `word` specifies the number of bytes between delimiters.
///
/// For example, delimiter=':' and word=2 produces "0001:0203:0405"
///
/// \param  data pointer to raw memory
/// \param  length size of input buffer
/// \param  delimiter character to insert to format the hex string
/// \param  word number of bytes between delimiters
/// \return hexadecimal string
std::string RawDataToHex(const void *data, size_t length, char delimiter,
                         int word = 1);

/// Convert a hexadecimal string to raw memory. Only write up to `length` bytes.
///
/// Ignore non-hex digits and the odd final hex digit. If there are fewer than
/// `length` bytes converted from the hex string, set the remaining bytes to
/// zero. If `error` is not null, set value to true when there are non-hex
/// digits or an odd number of hex digits.
///
/// \param  hex string containing hexadecimal characters
/// \param  data output buffer
/// \param  length size of output buffer
/// \param  error ptr to optional boolean error result
/// \return number of bytes resulting from hexadecimal string
size_t HexToRawData(const std::string &hex, void *data, size_t length,
                    bool *error = nullptr);

/// Convert a hexadecimal string to raw memory. Ignore non-hex digits and the
/// odd final hex digit.
///
/// \param  hex string containing hexadecimal characters
/// \return string containing raw bytes
std::string HexToRawData(const std::string &hex);

/// Return true if memory block is filled with given byte value.
///
/// \param  data pointer to memory block
/// \param  length length of memory block
/// \param  fill fill byte
/// \return true if memory block is filled with given byte value.
bool IsMemFilled(const void *data, size_t length, char fill);

/// Return true if pointer is aligned to specified byte boundary.
///
/// For example, to check if pointer is 64-bit aligned:
///    IsPtrAligned<8>(ptr)
///
/// \param  pointer
/// \return true if pointer is aligned.
template <unsigned ByteBoundary>
constexpr bool IsPtrAligned(const void *ptr) {
  return (reinterpret_cast<uintptr_t>(ptr) & (ByteBoundary - 1)) == 0;
}

/// Return a constructed unique_ptr for the specified type by forwarding
/// arguments to the constructor.
///
/// \return unique ptr to new object.
template <class T, class... Args>
std::unique_ptr<T> MakeUniquePtr(Args &&... args) {
  static_assert(!std::is_array<T>::value, "Only supports non-array types.");
  return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}

} // </namespace ofp>

// Place forward declarations of YAML classes here.

namespace llvm { // <namespace llvm>
namespace yaml { // <namespace yaml>

template <class T>
struct MappingTraits;

} // </namespace yaml>
} // </namespace llvm>

#endif // OFP_TYPES_H

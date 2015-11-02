// Copyright 2014-present Bill Fisher. All rights reserved.

#ifndef OFP_MACADDRESS_H_
#define OFP_MACADDRESS_H_

#include "ofp/types.h"
#include "ofp/array.h"

namespace ofp {

class MacAddress {
 public:
  enum { Length = 6 };

  using ArrayType = std::array<UInt8, Length>;

  MacAddress() : addr_{} {}
  /* implicit NOLINT */ MacAddress(const std::string &s);

  bool parse(const std::string &s);

  bool valid() const { return !IsMemFilled(addr_.data(), sizeof(addr_), '\0'); }

  bool isMulticast() const { return (addr_[0] & 0x01); }
  bool isBroadcast() const {
    return IsMemFilled(addr_.data(), sizeof(addr_), '\xff');
  }
  void setAllOnes() { std::memset(addr_.data(), 0xFF, sizeof(addr_)); }

  void clear() { std::memset(addr_.data(), 0, sizeof(addr_)); }

  std::string toString() const;

  const ArrayType &toArray() const { return addr_; }

  bool operator==(const MacAddress &rhs) const { return addr_ == rhs.addr_; }

  bool operator!=(const MacAddress &rhs) const { return !(*this == rhs); }

 private:
  ArrayType addr_;
};

static_assert(sizeof(MacAddress) == 6, "Unexpected size.");
static_assert(IsStandardLayout<MacAddress>(), "Expected standard layout.");
static_assert(IsTriviallyCopyable<MacAddress>(),
              "Expected trivially copyable.");

inline std::ostream &operator<<(std::ostream &os, const MacAddress &value) {
  return os << value.toString();
}

}  // namespace ofp

namespace std {

template <>
struct hash<ofp::MacAddress> {
  size_t operator()(const ofp::MacAddress &rhs) const {
    std::hash<ofp::MacAddress::ArrayType> hasher;
    return hasher(rhs.toArray());
  }
};

}  // namespace std

#endif  // OFP_MACADDRESS_H_
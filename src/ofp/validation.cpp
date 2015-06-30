// Copyright 2014-present Bill Fisher. All rights reserved.

#include "ofp/validation.h"
#include "ofp/log.h"
#include "ofp/message.h"
#include "ofp/messageinfo.h"

using namespace ofp;

Validation::Validation(const Message *msg, OFPErrorCode *error)
    : msg_{msg}, error_{error} {
  assert(error_);
  *error_ = OFPEC_UNKNOWN_FLAG;
  if (msg) {
    version_ = msg->version();
    length_ = msg->size();
  } else {
    version_ = 0;
    length_ = 0;
  }
}

void Validation::messageSizeIsInvalid() {
  *error_ = OFPBRC_BAD_LEN;
  setErrorMessage("Message size is invalid");
}

void Validation::messageTypeIsNotSupported() {
  *error_ = OFPBRC_BAD_TYPE;
  setErrorMessage("Message type is not supported");
}

void Validation::multipartTypeIsNotSupported() {
  *error_ = OFPBRC_BAD_MULTIPART;
  setErrorMessage("Multipart type is not supported");
}

void Validation::multipartTypeIsNotSupportedForVersion() {
  *error_ = OFPBRC_BAD_MULTIPART;
  setErrorMessage("Multipart type is not supported for this protocol version");
}

void Validation::multipartSizeHasImproperAlignment() {
  *error_ = OFPBRC_BAD_LEN;
  setErrorMessage("Multipart size has improper alignment");
}

void Validation::lengthRemainingIsInvalid(const UInt8 *ptr,
                                          size_t expectedLength) {
  *error_ = OFPBRC_BAD_LEN;
  setErrorMessage("Length remaining is invalid", offset(ptr));
}

void Validation::rangeSizeHasImproperAlignment(const UInt8 *ptr, ProtocolIteratorType type) {
  *error_ = OFPBRC_BAD_LEN;
  log::info("rangeSizeHasImproperAlignment", (int)type);
  setErrorMessage("Range size has improper alignment", offset(ptr));
}

void Validation::rangeDataHasImproperAlignment(const UInt8 *ptr,
                                               size_t alignment) {
  *error_ = OFPBRC_BAD_LEN;
  setErrorMessage("Range data has improper alignment", offset(ptr));
}

void Validation::rangeElementSizeIsTooSmall(const UInt8 *ptr, size_t minSize) {
  *error_ = OFPBRC_BAD_LEN;
  setErrorMessage("Range element size is too small", offset(ptr));
}

void Validation::rangeElementSizeHasImproperAlignment(const UInt8 *ptr,
                                                      size_t elemSize,
                                                      size_t alignment) {
  *error_ = OFPBRC_BAD_LEN;
  setErrorMessage("Range element size has improper alignment", offset(ptr));
}

void Validation::rangeElementSizeOverrunsEnd(const UInt8 *ptr,
                                             size_t jumpSize) {
  *error_ = OFPBRC_BAD_LEN;
  setErrorMessage("Range element size overruns end", offset(ptr));
}

void Validation::rangeSizeIsNotMultipleOfElementSize(const UInt8 *ptr,
                                                     size_t elementSize) {
  *error_ = OFPBRC_BAD_LEN;
  setErrorMessage("Range size is not multiple of element size", offset(ptr));
}

size_t Validation::offset(const UInt8 *ptr) const {
  if (!msg_ || ptr < msg_->data()) {
    return 0xFFFFFFFF;
  }
  return Unsigned_cast(ptr - msg_->data());
}

#if 0
std::string Validation::hexContext(const UInt8 *ptr) const {
  assert(msg_);

  const UInt8 *data = msg_->data();
  if ((ptr < data) || (ptr >= data + length_)) {
    return "<out of range>";
  }

  size_t len = 16;
  if (Unsigned_cast((data + length_) - ptr) < len) {
    len = Unsigned_cast((data + length_) - ptr);
  }

  return RawDataToHex(ptr, len);
}

void Validation::logContext(const UInt8 *ptr) const {
  assert(msg_ != nullptr || length_ == 0);

  if (length_ >= sizeof(Header)) {
    const Header *header = msg_->header();
    OFPType type = header->type();
    UInt8 version = header->version();

    std::ostringstream oss;
    if (length_ >= 16 &&
        (type == OFPT_MULTIPART_REQUEST || type == OFPT_MULTIPART_REPLY)) {
      OFPMultipartType mpType =
          *reinterpret_cast<const Big<OFPMultipartType> *>(msg_->data() +
                                                           sizeof(Header));
      oss << type << '.' << mpType << ".v" << static_cast<int>(version);
    } else {
      oss << type << ".v" << static_cast<int>(version);
    }

    log::info("  Message type:", oss.str());
    if (ptr) {
      log::info("  Context(16 bytes):", hexContext(ptr));
    }
  }
}
#endif //0

void Validation::setErrorMessage(const char *errorMessage, size_t errorOffset) {
  log::info("Validation failed:", errorMessage, std::make_pair("offset", errorOffset));

  if (msg_) {
    if (MessageInfo *info = msg_->info()) {
      info->setErrorMessage(errorMessage);
    }
  }
}

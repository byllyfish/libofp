// Copyright 2014-present Bill Fisher. All rights reserved.

#ifndef OFP_SYS_ASIO_OPENSSL_INIT_H_
#define OFP_SYS_ASIO_OPENSSL_INIT_H_

#include <asio/ssl/detail/openssl_init.hpp>

namespace asio {
namespace ssl {
namespace detail {

template <>
class openssl_init<> : private openssl_init_base {
 public:
  // Constructor.
  openssl_init() : ref_(instance()) {}

  // Destructor.
  ~openssl_init() {}

#if !defined(SSL_OP_NO_COMPRESSION) && (OPENSSL_VERSION_NUMBER >= 0x00908000L)
  using openssl_init_base::get_null_compression_methods;
#endif  // !defined(SSL_OP_NO_COMPRESSION)
        // && (OPENSSL_VERSION_NUMBER >= 0x00908000L)

  using openssl_init_base::instance;

 private:
  asio::detail::shared_ptr<do_init> ref_;
};

}  // namespace detail
}  // namespace ssl
}  // namespace asio

#endif  // OFP_SYS_ASIO_OPENSSL_INIT_H_
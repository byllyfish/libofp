// Copyright 2015-present Bill Fisher. All rights reserved.

#ifndef OFP_YAML_YSCHEMA_H_
#define OFP_YAML_YSCHEMA_H_

#include <set>
#include "ofp/yaml/yllvm.h"

namespace ofp {
namespace yaml {

OFP_BEGIN_IGNORE_PADDING

class Schema {
 public:
  /// Construct using constant/immutable/never-deleted C string (careful!)
  explicit Schema(const char *const schema) { init(schema); }

  explicit Schema(const std::string &schema) : buf_{schema} {
    init(buf_.c_str());
  }

  llvm::StringRef type() const { return type_; }
  llvm::StringRef name() const { return name_; }
  llvm::StringRef value() const { return value_; }

  bool equals(llvm::StringRef s) const;

  std::set<std::string> dependsOnSchemas() const;

  void print(std::ostream &os) const;
  void printValue(std::ostream &os, unsigned indent = 0) const;

 private:
  std::string buf_;
  llvm::StringRef type_;
  llvm::StringRef name_;
  llvm::StringRef value_;
  bool isObject_ = false;

  void init(const char *const schema);

  static std::string MakeSchemaString(
      const char *const name, const std::vector<llvm::StringRef> &values,
      size_t size);

  template <class Type>
  friend std::string MakeSchema(const char *const name);
};

OFP_END_IGNORE_PADDING

typedef std::string (*SchemaMakerFunction)(const char *const);

template <class Type>
std::string MakeSchema(const char *const name) {
  auto values = llvm::yaml::ScalarTraits<Type>::converter.listAll();
  return Schema::MakeSchemaString(name, values, sizeof(Type));
}

}  // namespace yaml
}  // namespace ofp

#endif  // OFP_YAML_YSCHEMA_H_
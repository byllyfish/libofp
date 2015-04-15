// Copyright 2015-present Bill Fisher. All rights reserved.

#ifndef TOOLS_OFPX_OFPX_HELP_H_
#define TOOLS_OFPX_OFPX_HELP_H_

#include "./ofpx.h"

namespace ofp {
namespace yaml {

class Schema;

}  // namespace yaml
}  // namespace ofp

namespace ofpx {

using ofp::yaml::Schema;

// ofpx help [--fields|-f] [--actions|-a] [--instructions|-i] [--messages|-m] [args...]
//
// Access built-in help features.
//
//   --fields         List supported match fields.
//   --actions        List supported actions.
//   --instructions   List supported instructions.
//   --messages       List supported message types.
//   --enums          List supported enumeration types.
//   --flags          List supported flags.
//   --mixed          List supported mixed types.

class Help : public Subprogram {
 public:
  enum class ExitStatus { Success = 0 };

  int run(int argc, const char *const *argv) override;

  // Declare separate constructor/destructor to compile `schemas_`.
  Help();
  ~Help();

 private:
  std::vector<std::unique_ptr<Schema>> schemas_;

  void loadSchemas();
  Schema *findSchema(const std::string &key);
  Schema *findNearestSchema(const std::string &key);

  void listFields();
  void listSchemas(const std::string &type);
  void printSchema(const std::string &key);

  void dumpSchemas();

  static const char *translateFieldType(const char *type);

  // --- Command-line Arguments ---
  cl::opt<bool> fields_{"fields", cl::desc("List supported match fields.")};
  cl::opt<bool> actions_{"actions", cl::desc("List supported actions.")};
  cl::opt<bool> instructions_{"instructions", cl::desc("List supported instructions.")};
  cl::opt<bool> messages_{"messages", cl::desc("List supported messages.")};
  cl::opt<bool> enums_{"enums", cl::desc("List supported enumerated types.")};
  cl::opt<bool> dump_{"dump", cl::desc("List all schema names.")};
  cl::list<std::string> args_{cl::Positional, cl::desc("<Args>")};

  // --- Argument Aliases ---
  cl::alias fAlias_{"f", cl::desc("Alias for -fields"), cl::aliasopt(fields_)};
  cl::alias aAlias_{"a", cl::desc("Alias for -actions"), cl::aliasopt(actions_)};
  cl::alias iAlias_{"i", cl::desc("Alias for -instructions"), cl::aliasopt(instructions_)};
  cl::alias mAlias_{"m", cl::desc("Alias for -messages"), cl::aliasopt(messages_)};
  cl::alias eAlias_{"e", cl::desc("Alias for -enums"), cl::aliasopt(enums_)};
};

}  // namespace ofpx

#endif  // TOOLS_OFPX_OFPX_HELP_H_

// Copyright 2014-present Bill Fisher. All rights reserved.

#include "./ofpx_help.h"
#include <iostream>
#include <iomanip>
#include "ofp/oxmtype.h"
#include "ofp/yaml/yhello.h"
#include "ofp/yaml/yerror.h"
#include "ofp/yaml/yecho.h"
#include "ofp/yaml/yfeaturesreply.h"
#include "ofp/yaml/ygetconfigreply.h"
#include "ofp/yaml/ysetconfig.h"
#include "ofp/yaml/ypacketin.h"
#include "ofp/yaml/ypacketout.h"
#include "ofp/yaml/yflowmod.h"
#include "ofp/yaml/yflowremoved.h"
#include "ofp/yaml/yportmod.h"
#include "ofp/yaml/yheaderonly.h"
#include "ofp/yaml/yschema.h"
#include "ofp/yaml/yportstatus.h"
#include "ofp/yaml/ygroupmod.h"
#include "ofp/yaml/ytablemod.h"
#include "ofp/yaml/ymultipartrequest.h"
#include "ofp/yaml/ymultipartreply.h"
#include "ofp/yaml/yqueuegetconfigrequest.h"
#include "ofp/yaml/yqueuegetconfigreply.h"
#include "ofp/yaml/yrolerequest.h"
#include "ofp/yaml/yrolereply.h"
#include "ofp/yaml/ygetasyncreply.h"
#include "ofp/yaml/ysetasync.h"
#include "ofp/yaml/ymetermod.h"
#include "ofp/yaml/yrolestatus.h"
#include "ofp/yaml/ybundleaddmessage.h"
#include "ofp/yaml/ybundlecontrol.h"

using namespace ofpx;

// Declare separate constructor/destructor to compile `schemas_`.
Help::Help() = default;
Help::~Help() = default;

OFP_BEGIN_IGNORE_GLOBAL_CONSTRUCTOR

static const char *const kMessageSchemas[] = {
    llvm::yaml::kHelloSchema, llvm::yaml::kErrorSchema,
    llvm::yaml::kEchoRequestSchema, llvm::yaml::kEchoReplySchema,
    llvm::yaml::kFeaturesRequestSchema, llvm::yaml::kFeaturesReplySchema,
    llvm::yaml::kGetConfigRequestSchema, llvm::yaml::kGetConfigReplySchema,
    llvm::yaml::kSetConfigSchema, llvm::yaml::kPacketInSchema,
    llvm::yaml::kFlowRemovedSchema, llvm::yaml::kPortStatusSchema,
    llvm::yaml::kPacketOutSchema, llvm::yaml::kFlowModSchema,
    llvm::yaml::kGroupModSchema, llvm::yaml::kPortModSchema,
    llvm::yaml::kTableModSchema, llvm::yaml::kMultipartRequestSchema,
    llvm::yaml::kMultipartReplySchema, llvm::yaml::kBarrierRequestSchema,
    llvm::yaml::kBarrierReplySchema, llvm::yaml::kQueueGetConfigRequestSchema,
    llvm::yaml::kQueueGetConfigReplySchema, llvm::yaml::kRoleRequestSchema,
    llvm::yaml::kRoleReplySchema, llvm::yaml::kGetAsyncRequestSchema,
    llvm::yaml::kGetAsyncReplySchema, llvm::yaml::kSetAsyncSchema,
    llvm::yaml::kMeterModSchema, llvm::yaml::kRoleStatusSchema,
    // llvm::yaml::kTableStatusSchema,
    // llvm::yaml::kRequestForwardSchema,
    llvm::yaml::kBundleControlSchema, llvm::yaml::kBundleAddMessageSchema,
};

static const char *const kMultipartSchemas[] = {
    llvm::yaml::kMPDescSchema,
};

static const char *const kInstructionSchemas[] = {
    llvm::yaml::kGotoTableSchema, llvm::yaml::kWriteMetadataSchema,
    llvm::yaml::kWriteActionsSchema, llvm::yaml::kApplyActionsSchema,
    llvm::yaml::kClearActionsSchema, llvm::yaml::kMeterSchema,
    llvm::yaml::kExperimenterInstructionSchema,
};

static const char *const kActionSchemas[] = {
    llvm::yaml::kCopyTTLOutSchema, llvm::yaml::kCopyTTLInSchema,
    llvm::yaml::kDecMPLSTTLSchema, llvm::yaml::kPopVLANSchema,
    llvm::yaml::kDecNwTTLSchema, llvm::yaml::kPopPBBSchema,
    llvm::yaml::kOutputSchema, llvm::yaml::kSetMPLSTTLSchema,
    llvm::yaml::kPushVLANSchema, llvm::yaml::kPushMPLSSchema,
    llvm::yaml::kPopMPLSSchema, llvm::yaml::kSetQueueSchema,
    llvm::yaml::kGroupSchema, llvm::yaml::kSetNwTTLSchema,
    llvm::yaml::kPushPBBSchema, llvm::yaml::kSetFieldSchema,
    llvm::yaml::kExperimenterActionSchema,
};

using SchemaPair = std::pair<ofp::yaml::SchemaMakerFunction, const char *>;

static SchemaPair kEnumSchemas[] = {
    {ofp::yaml::MakeSchema<ofp::OFPFlowModCommand>, "Enum/FlowModCommand"},
    {ofp::yaml::MakeSchema<ofp::OFPPacketInReason>, "Enum/PacketInReason"},
    {ofp::yaml::MakeSchema<ofp::OFPPortStatusReason>, "Enum/PortStatusReason"},
    {ofp::yaml::MakeSchema<ofp::OFPFlowRemovedReason>,
     "Enum/FlowRemovedReason"},
    {ofp::yaml::MakeSchema<ofp::OFPControllerRole>, "Enum/ControllerRole"},
    {ofp::yaml::MakeSchema<ofp::OFPMeterModCommand>, "Enum/MeterModCommand"},
    {ofp::yaml::MakeSchema<ofp::OFPErrorType>, "Enum/ErrorType"},
    {ofp::yaml::MakeSchema<ofp::OFPFlowUpdateEvent>, "Enum/FlowUpdateEvent"},
    {ofp::yaml::MakeSchema<ofp::OFPErrorCode>, "Enum/ErrorCode"},
};

static SchemaPair kMixedSchemas[] = {
    {ofp::yaml::MakeSchema<ofp::PortNumber>, "Mixed/PortNumber"},
    {ofp::yaml::MakeSchema<ofp::GroupNumber>, "Mixed/GroupNumber"},
    {ofp::yaml::MakeSchema<ofp::BufferID>, "Mixed/BufferID"},
    {ofp::yaml::MakeSchema<ofp::TableNumber>, "Mixed/TableNumber"},
    {ofp::yaml::MakeSchema<ofp::ControllerMaxLen>, "Mixed/ControllerMaxLen"},
};

// Translate "BigNN" to "UIntNN" for documentation purposes.
static std::pair<const char *, const char *> sFieldTypeMap[] = {
    {"Big8", "UInt8"},
    {"Big16", "UInt16"},
    {"Big32", "UInt32"},
    {"Big64", "UInt64"}};

OFP_END_IGNORE_GLOBAL_CONSTRUCTOR

int Help::run(int argc, const char *const *argv) {
  parseCommandLineOptions(
      argc, argv, "Provide information about the OpenFlow YAML schema\n");
  loadSchemas();

  if (fields_) {
    listFields();
  } else if (messages_) {
    listSchemas("Message");
  } else if (multipart_) {
    listSchemas("Multipart");
  } else if (instructions_) {
    listSchemas("Instruction");
  } else if (actions_) {
    listSchemas("Action");
  } else if (enums_) {
    listSchemas("Enum");
  } else if (dump_) {
    dumpSchemas();
  } else if (!args_.empty()) {
    for (auto &arg : args_) {
      printSchema(arg);
    }
  } else {
    cl::PrintHelpMessage(false, true);
    return 1;
  }

  return 0;
}

void Help::loadSchemas() {
  for (auto &schema : kMessageSchemas) {
    schemas_.emplace_back(new Schema{schema});
  }

  for (auto &schema : kMultipartSchemas) {
    schemas_.emplace_back(new Schema{schema});
  }

  for (auto &schema : kInstructionSchemas) {
    schemas_.emplace_back(new Schema{schema});
  }

  for (auto &schema : kActionSchemas) {
    schemas_.emplace_back(new Schema{schema});
  }

  for (auto &p : kEnumSchemas) {
    schemas_.emplace_back(new Schema{p.first(p.second)});
  }

  for (auto &p : kMixedSchemas) {
    schemas_.emplace_back(new Schema{p.first(p.second)});
  }
}

Schema *Help::findSchema(const std::string &key) {
  auto iter = std::find_if(
      schemas_.begin(), schemas_.end(),
      [&key](std::unique_ptr<Schema> &schema) { return schema->equals(key); });

  return iter != schemas_.end() ? iter->get() : nullptr;
}

Schema *Help::findNearestSchema(const std::string &key) {
  std::string s = llvm::StringRef{key}.upper();

  unsigned minDistance = 0xffff;
  Schema *minSchema = nullptr;

  for (auto &schema : schemas_) {
    auto buf = schema->name().upper();
    auto name = llvm::StringRef{buf};
    if (name.startswith(s))
      return schema.get();
    unsigned dist = name.edit_distance(s, true, minDistance);
    if (dist < minDistance) {
      minDistance = dist;
      minSchema = schema.get();
    }
  }

  return minSchema;
}

void Help::listFields() {
  // Determine the maximum width of the name and type fields.

  int nameWidth = 0;
  int typeWidth = 0;

  for (size_t i = 0; i < ofp::OXMTypeInfoArraySize; ++i) {
    const ofp::OXMTypeInfo *info = &ofp::OXMTypeInfoArray[i];
    auto typeStr = translateFieldType(info->type);
    nameWidth = std::max(nameWidth, static_cast<int>(std::strlen(info->name)));
    typeWidth = std::max(typeWidth, static_cast<int>(std::strlen(typeStr)));
  }

  // Print out the name, type and description of each field.

  for (size_t i = 0; i < ofp::OXMTypeInfoArraySize; ++i) {
    const ofp::OXMTypeInfo *info = &ofp::OXMTypeInfoArray[i];
    auto typeStr = translateFieldType(info->type);
    std::cout << std::setw(nameWidth) << std::left << info->name << " | "
              << std::setw(typeWidth) << std::left << typeStr << " | "
              << info->description << '\n';
  }
}

void Help::listSchemas(const std::string &type) {
  // Print out the name of each schema with the specified type.
  for (auto &schema : schemas_) {
    if (schema->type() == type) {
      std::cout << schema->name() << '\n';
    }
  }
}

void Help::printSchema(const std::string &key) {
  auto schema = findSchema(key);
  if (schema) {
    schema->print(std::cout);

    for (auto &s : schema->dependsOnSchemas()) {
      auto depSchema = findSchema(s);
      if (depSchema) {
        depSchema->print(std::cout);
      }
    }

  } else {
    schema = findNearestSchema(key);
    if (schema) {
      std::cerr << "Unknown command line argument '" << key
                << "'. Did you mean '" << schema->name() << "'?" << '\n';
    }
  }
}

/// Print out the type/name for each schema.
void Help::dumpSchemas() {
  for (auto &schema : schemas_) {
    std::cout << schema->type().lower() << '/' << schema->name() << '\n';
  }
}

const char *Help::translateFieldType(const char *type) {
  for (auto &entry : sFieldTypeMap) {
    if (std::strcmp(type, entry.first) == 0) {
      return entry.second;
    }
  }
  return type;
}

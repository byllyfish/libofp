#ifndef OFP_YAML_YCONTEXT_H_
#define OFP_YAML_YCONTEXT_H_

#include "ofp/types.h"

namespace ofp {
namespace yaml {

class Encoder;
class Decoder;

// A ptr to the YamlContext is passed as the IO's context for YAML operations.
// The YamlContext provides access to the current encoder, decoder and protocol
// version. The protocol version is only available when decoding a binary 
// OpenFlow packet.

struct YamlContext {
    explicit YamlContext(Encoder *enc) : encoder{enc}, decoder{nullptr}, version{0} { assert(validate()); }
    explicit YamlContext(Decoder *dec, UInt8 ver) : encoder{nullptr}, decoder{dec}, version{ver} { assert(validate()); }

    Encoder *encoder;
    Decoder *decoder;
    UInt8 version;
    const UInt8 magic[7] = "contxt";

    bool validate() const { return std::memcmp(magic, "contxt", 7) == 0; }

    static Encoder *GetEncoder(void *context);
};

Encoder *GetEncoderFromContext(llvm::yaml::IO &io);
Decoder *GetDecoderFromContext(llvm::yaml::IO &io);
UInt8 GetVersionFromContext(llvm::yaml::IO &io);

}  // namespace yaml
}  // namespace ofp

#endif // OFP_YAML_YCONTEXT_H_

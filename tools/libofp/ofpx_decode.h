// Copyright (c) 2015-2016 William W. Fisher (at gmail dot com)
// This file is distributed under the MIT License.

#ifndef TOOLS_LIBOFP_OFPX_DECODE_H_
#define TOOLS_LIBOFP_OFPX_DECODE_H_

#include <map>
#include "./ofpx.h"
#include "ofp/messageinfo.h"
#include "ofp/timestamp.h"

namespace ofpx {

// libofp decode [<options>] [<Input files>]
//
// Decode binary OpenFlow messages in the input files and translate each
// message to human-readable YAML output. If there is an invalid message,
// stop and report an error.
//
// If no input files are specified, use standard input (stdin). A single hyphen
// also represents stdin.
//
//   --json (-j)           Write compact JSON output instead of YAML.
//   --json-array          Write output as a valid JSON array.
//   --silent (-s)         Quiet mode; suppress normal output.
//   --silent-error        Suppress error output for invalid messages.
//   --invert-check (-v)   Expect invalid messages only.
//   --keep-going (-k)     Continue processing messages after errors.
//   --verify-output (-V)  Verify output by translating it back to binary.
//   --use-findx           Use metadata from tcpflow '.findx' files.
//   --pkt-decode          Include _pkt_decode in PacketIn/PacketOut decodes.
//   --include-filename    Include file name in all decodes.
//   --output=<file> (-o)  Write output to specified file instead of stdout.
//   --pcap-device=<device> Reassemble OpenFlow messages from specified device.
//   --pcap-format          Treat input files as .pcap format.
//   --pcap-debug           Write reassembled TCP streams to /tmp/libofp.msgs.
//
// Usage:
//
// To decode a file of binary OpenFlow messages to YAML:
//
//     libofp decode "filename"
//
// To decode a file of binary OpenFlow messages to JSON:
//
//     libofp decode --json "filename"
//
// To process a file of invalid binary OpenFlow messages (e.g. fuzz test). This
// will report an error if any message is structurally valid as an OpenFlow
// message:
//
//     libofp decode --invert-check "filename"
//

OFP_BEGIN_IGNORE_PADDING

class Decode : public Subprogram {
 public:
  enum class ExitStatus {
    Success = 0,
    FileOpenFailed = FileOpenFailedExitStatus,
    DecodeFailed = MinExitStatus,
    DecodeSucceeded,
    VerifyOutputFailed,
    MessageReadFailed,
    IndexReadFailed
  };

  int run(int argc, const char *const *argv) override;

 private:
  std::string currentFilename_;
  std::ostream *output_ = nullptr;
  ofp::MessageInfo sessionInfo_;
  bool jsonNeedComma_ = false;

  using EndpointPair = std::pair<ofp::IPv6Endpoint, ofp::IPv6Endpoint>;
  std::map<EndpointPair, ofp::UInt64> sessionIdMap_;
  ofp::UInt64 nextSessionId_ = 0;

  ExitStatus decodeFiles();
  ExitStatus decodeFile(const std::string &filename);
  ExitStatus decodeMessages(std::istream &input);
  ExitStatus decodeMessagesWithIndex(std::istream &input, std::istream &index);
  ExitStatus decodePcapDevice(const std::string &device);
  ExitStatus decodePcapFiles();
  ExitStatus checkError(std::istream &input, std::streamsize readLen,
                        bool header);
  ExitStatus decodeOneMessage(const ofp::Message *message,
                              const ofp::Message *originalMessage);

  bool equalMessages(ofp::ByteRange origData, ofp::ByteRange newData) const;

  static bool parseIndexLine(const llvm::StringRef &line, size_t *pos,
                             ofp::Timestamp *timestamp, size_t *length);

  void setCurrentFilename(const std::string &filename);
  bool parseFilename(const std::string &filename, ofp::MessageInfo *info);
  ofp::UInt64 lookupSessionId(const ofp::IPv6Endpoint &src,
                              const ofp::IPv6Endpoint &dst);

  static void pcapMessageCallback(ofp::Message *message, void *context);

  // --- Command-line Arguments (Order is important here.) ---
  cl::opt<bool> json_{"json",
                      cl::desc("Write compact JSON output instead of YAML")};
  cl::opt<bool> jsonArray_{"json-array",
                           cl::desc("Write output as a valid JSON array")};
  cl::opt<bool> silent_{"silent",
                        cl::desc("Quiet mode; suppress normal output")};
  cl::opt<bool> silentError_{
      "silent-error", cl::desc("Suppress error output for invalid messages")};
  cl::opt<bool> invertCheck_{"invert-check",
                             cl::desc("Expect invalid messages only")};
  cl::opt<bool> keepGoing_{
      "keep-going", cl::desc("Continue processing messages after errors")};
  cl::opt<bool> verifyOutput_{
      "verify-output",
      cl::desc("Verify output by translating it back to binary")};
  cl::opt<bool> useFindx_{"use-findx",
                          cl::desc("Use metadata from tcpflow '.findx' files")};
  cl::opt<bool> pktDecode_{
      "pkt-decode",
      cl::desc("Include _pkt_decode in PacketIn/PacketOut decodes")};
  cl::opt<bool> includeFilename_{"include-filename",
                                 cl::desc("Include file name in all decodes")};
  cl::opt<std::string> outputFile_{
      "output", cl::desc("Write output to specified file instead of stdout"),
      cl::ValueRequired};
  cl::opt<std::string> pcapDevice_{
      "pcap-device",
      cl::desc("Reassemble OpenFlow messages from specified device"),
      cl::ValueRequired};
  cl::opt<bool> pcapFormat_{"pcap-format",
                            cl::desc("Treat input files as .pcap format")};
  cl::opt<bool> pcapDebug_{"pcap-debug", cl::desc("Write reassembled TCP streams to /tmp/libofp.msgs.")};
  cl::list<std::string> inputFiles_{cl::Positional, cl::desc("<Input files>")};

  // --- Argument Aliases (May be grouped into one argument) ---
  cl::alias jAlias_{"j", cl::desc("Alias for -json"), cl::aliasopt(json_),
                    cl::Grouping};
  cl::alias sAlias_{"s", cl::desc("Alias for -silent"), cl::aliasopt(silent_),
                    cl::Grouping};
  cl::alias vAlias_{"v", cl::desc("Alias for -invert-check"),
                    cl::aliasopt(invertCheck_), cl::Grouping};
  cl::alias kAlias_{"k", cl::desc("Alias for -keep-going"),
                    cl::aliasopt(keepGoing_), cl::Grouping};
  cl::alias VAlias_{"V", cl::desc("Alias for -verify-output"),
                    cl::aliasopt(verifyOutput_), cl::Grouping};
  cl::alias oAlias_{"o", cl::desc("Alias for -output"),
                    cl::aliasopt(outputFile_)};
};

OFP_END_IGNORE_PADDING

}  // namespace ofpx

#endif  // TOOLS_LIBOFP_OFPX_DECODE_H_

// Copyright (c) 2015-2018 William W. Fisher (at gmail dot com)
// This file is distributed under the MIT License.

#ifndef OFP_YAML_YMETERBANDS_H_
#define OFP_YAML_YMETERBANDS_H_

#include "ofp/meterbands.h"

namespace ofp {
namespace detail {

struct MeterBandInserter {};

}  // namespace detail
}  // namespace ofp

namespace llvm {
namespace yaml {

const char *const kMeterBandDropSchema = R"""({MeterBand/DROP}
type: DROP
rate: UInt32
burst_size: !opt UInt32    # default=0
)""";

const char *const kMeterBandDscpRemarkSchema = R"""({MeterBand/DSCP_REMARK}
type: DSCP_REMARK
rate: UInt32
burst_size: !opt UInt32    # default=0
prec_level: !opt UInt8     # default=0
)""";

const char *const kMeterBandExperimenterSchema = R"""({MeterBand/EXPERIMENTER}
type: EXPERIMENTER
rate: UInt32
burst_size: !opt UInt32   # default=0
experimenter: UInt32
)""";

template <>
struct MappingTraits<ofp::MeterBandIterator::Element> {
  static void mapping(IO &io, ofp::MeterBandIterator::Element &elem) {
    using namespace ofp;

    OFPMeterBandType meterType = elem.type();
    io.mapRequired("type", meterType);

    switch (meterType) {
      case MeterBandDrop::type(): {
        auto &p = elem.meterBand<MeterBandDrop>();
        Hex32 rate = p.rate();
        Hex32 burstSize = p.burstSize();
        io.mapRequired("rate", rate);
        io.mapRequired("burst_size", burstSize);
        break;
      }
      case MeterBandDscpRemark::type(): {
        auto &p = elem.meterBand<MeterBandDscpRemark>();
        Hex32 rate = p.rate();
        Hex32 burstSize = p.burstSize();
        Hex8 precLevel = p.precLevel();
        io.mapRequired("rate", rate);
        io.mapRequired("burst_size", burstSize);
        io.mapRequired("prec_level", precLevel);
        break;
      }
      case MeterBandExperimenter::type(): {
        auto &p = elem.meterBand<MeterBandExperimenter>();
        Hex32 rate = p.rate();
        Hex32 burstSize = p.burstSize();
        Hex32 experimenter = p.experimenter();
        io.mapRequired("rate", rate);
        io.mapRequired("burst_size", burstSize);
        io.mapRequired("experimenter", experimenter);
        break;
      }
      default:
        break;
    }
  }
};

template <>
struct MappingTraits<ofp::detail::MeterBandInserter> {
  static void mapping(IO &io, ofp::detail::MeterBandInserter &inserter) {
    using namespace ofp;

    MeterBandList &list = Ref_cast<MeterBandList>(inserter);

    OFPMeterBandType meterType = OFPMBT_NONE;
    io.mapRequired("type", meterType);

    switch (meterType) {
      case MeterBandDrop::type(): {
        UInt32 rate, burstSize;
        io.mapRequired("rate", rate);
        io.mapOptional("burst_size", burstSize, 0);
        list.add(MeterBandDrop{rate, burstSize});
        break;
      }
      case MeterBandDscpRemark::type(): {
        UInt32 rate, burstSize;
        UInt8 precLevel;
        io.mapRequired("rate", rate);
        io.mapOptional("burst_size", burstSize, 0);
        io.mapOptional("prec_level", precLevel, 0);
        list.add(MeterBandDscpRemark{rate, burstSize, precLevel});
        break;
      }
      case MeterBandExperimenter::type(): {
        UInt32 rate, burstSize, experimenter;
        io.mapRequired("rate", rate);
        io.mapOptional("burst_size", burstSize, 0);
        io.mapRequired("experimenter", experimenter);
        list.add(MeterBandExperimenter{rate, burstSize, experimenter});
        break;
      }
      default:
        break;
    }
  }
};

template <>
struct SequenceTraits<ofp::MeterBandRange> {
  using iterator = ofp::MeterBandIterator;

  static iterator begin(IO &io, ofp::MeterBandRange &range) {
    return range.begin();
  }

  static iterator end(IO &io, ofp::MeterBandRange &range) {
    return range.end();
  }

  static void next(iterator &iter, iterator iterEnd) { ++iter; }
};

template <>
struct SequenceTraits<ofp::MeterBandList> {
  static size_t size(IO &io, ofp::MeterBandList &list) { return 0; }

  static ofp::detail::MeterBandInserter &element(IO &io,
                                                 ofp::MeterBandList &list,
                                                 size_t index) {
    return Ref_cast<ofp::detail::MeterBandInserter>(list);
  }
};

}  // namespace yaml
}  // namespace llvm

#endif  // OFP_YAML_YMETERBANDS_H_

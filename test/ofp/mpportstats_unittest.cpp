#include "ofp/unittest.h"
#include "ofp/mpportstats.h"
#include "ofp/portstatsproperty.h"
using namespace ofp;

TEST(mpportstats, builder) {
    MPPortStatsBuilder statsBuilder;

    statsBuilder.setPortNo(0x11111112);
    statsBuilder.setRxPackets(0xAAAAAAAAAAAAAAA1);
    statsBuilder.setTxPackets(0xBBBBBBBBBBBBBBB1);
    statsBuilder.setDurationSec(0x22222221);
    statsBuilder.setDurationNSec(0x33333331);
    statsBuilder.setRxErrors(0x4444444444444441);
    statsBuilder.setTxErrors(0x5555555555555551);

    PortStatsPropertyEthernet eth;
    eth.setRxFrameErr(0x6666666666666661);
    eth.setRxOverErr(0x7777777777777771);
    eth.setRxCrcErr(0x8888888888888881);
    eth.setCollisions(0x9999999999999991);

    PropertyList props;
    props.add(eth);
    statsBuilder.setProperties(props);
/*
    PortStatsPropertyEthernet eth;
    eth.setRxFrameErr(0x5555555555555555);
    eth.setCollisions(0x6666666666666666);

    PropertyList props;
    props.add(eth);
    statsBuilder.setProperties(props);
*/

    {
        MemoryChannel channel{OFP_VERSION_4};
        statsBuilder.write(&channel);
        EXPECT_HEX("1111111200000000AAAAAAAAAAAAAAA1BBBBBBBBBBBBBBB100000000000000000000000000000000000000000000000000000000000000004444444444444441555555555555555166666666666666617777777777777771888888888888888199999999999999912222222133333331", channel.data(), channel.size());
    }

    {
        MemoryChannel channel{OFP_VERSION_3};
        statsBuilder.write(&channel);
        EXPECT_HEX("1111111200000000AAAAAAAAAAAAAAA1BBBBBBBBBBBBBBB10000000000000000000000000000000000000000000000000000000000000000444444444444444155555555555555516666666666666661777777777777777188888888888888819999999999999991", channel.data(), channel.size());
    }

    {
        MemoryChannel channel{OFP_VERSION_2};
        statsBuilder.write(&channel);
        EXPECT_HEX("1111111200000000AAAAAAAAAAAAAAA1BBBBBBBBBBBBBBB10000000000000000000000000000000000000000000000000000000000000000444444444444444155555555555555516666666666666661777777777777777188888888888888819999999999999991", channel.data(), channel.size());
    }

    {
        MemoryChannel channel{OFP_VERSION_1};
        statsBuilder.write(&channel);
        EXPECT_HEX("1112000000000000AAAAAAAAAAAAAAA1BBBBBBBBBBBBBBB10000000000000000000000000000000000000000000000000000000000000000444444444444444155555555555555516666666666666661777777777777777188888888888888819999999999999991", channel.data(), channel.size());
    }
}


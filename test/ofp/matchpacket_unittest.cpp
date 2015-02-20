#include "ofp/unittest.h"
#include "ofp/matchpacket.h"

using namespace ofp;


static void testPacket(const char *hex, const char *outHex) {
    std::string pkt = HexToRawData(hex);

    // Make sure that packet is aligned to a byte boundary as expected.
    // (Bypass Short-String Optimization)
    if (!IsPtrAligned(pkt.data(), 8))
        pkt.reserve(256);

    MatchPacket match{ByteRange{pkt.data() + 2, pkt.size() - 2}};
    EXPECT_HEX(outHex, match.data(), match.size());
}


TEST(matchpacket, ethernet) {
    testPacket("0000 FFFFFFFFFFFF0000000000010806", "80000606:FFFFFFFFFFFF 80000806:000000000001 80000A02:0806");
}

TEST(matchpacket, arp) {
    testPacket("0000 FFFFFFFFFFFF000000000001080600010800060400010000000000010A0000010000000000000A000002", "80000606:FFFFFFFFFFFF 80000806:000000000001 80000A02:0806 80002A02:0001 80002C04:0A000001 80002E04:0A000002 80003006:000000000001 80003206:000000000000");
}

TEST(matchpacket, tcpv4) {
    testPacket("0000 1111111111112222222222220800 45334444555500007706888899999999AAAAAAAA BBBBCCCCDDDDDDDDDDDDDDDDEEEEFFFFFFFFFFFF", "80000606:111111111111 80000806:222222222222 80000A02:0800 80001001:0C 80001201:03 80001401:06 80001604:99999999 80001804:AAAAAAAA 00013A01:77 80001A02:BBBB 80001C02:CCCC 00014402:0EEE");
}

TEST(matchpacket, udpv4) {
    testPacket("0000 1111111111112222222222220800 45334444555500007711888899999999AAAAAAAA BBBBCCCCDDDDEEEE", "80000606:111111111111 80000806:222222222222 80000A02:0800 80001001:0C 80001201:03 80001401:11 80001604:99999999 80001804:AAAAAAAA 00013A01:77 80001E02:BBBB 80002002:CCCC");
}

TEST(matchpacket, icmpv4) {
    testPacket("0000 1111111111112222222222220800 45334444555500007701888899999999AAAAAAAA BBCCDDDDDDDD", "80000606:111111111111 80000806:222222222222 80000A02:0800 80001001:0C 80001201:03 80001401:01 80001604:99999999 80001804:AAAAAAAA 00013A01:77 80002601:BB 80002801:CC");
}

TEST(matchpacket, icmpv6) {
    testPacket("0000 3333FF718C4C32FA25D4234186DD6000000000183AFF00000000000000000000000000000000FF0200000000000000000001FF718C4C87009A9E00000000FE80000000000000FC0CCDFFFE718C4C", "80000606:3333FF718C4C 80000806:32FA25D42341 80000A02:86DD 80001001:00 80001201:00 80001401:3A 80003410:00000000000000000000000000000000 80003610:FF0200000000000000000001FF718C4C 80003804:00000000 80003A01:87 80003C01:00 80004E02:0001");
}

TEST(matchpacket, udpv6) {
    testPacket("0000 11111111111122222222222286DD 63344444555511667777777777777777777777777777777788888888888888888888888888888888 BBBBCCCCDDDDEEEE", "80000606:111111111111 80000806:222222222222 80000A02:86DD 80001001:0C 80001201:03 80001401:11 80003410:77777777777777777777777777777777 80003610:88888888888888888888888888888888 80003804:00044444 80001E02:BBBB 80002002:CCCC 80004E02:0001");
}

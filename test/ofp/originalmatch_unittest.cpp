#include "ofp/unittest.h"
#include "ofp/originalmatch.h"
#include "ofp/matchbuilder.h"
#include "ofp/oxmfields.h"

using namespace ofp;
using namespace deprecated;


TEST(originalmatch, oxmrange)
{
	MatchBuilder oxmMatch;
    oxmMatch.add(OFB_IN_PORT{0xCCCCCCCC});
    oxmMatch.add(OFB_IPV4_DST{IPv4Address{"192.168.1.1"}});

	OriginalMatch match{oxmMatch.toRange()};

	EXPECT_HEX("00303FEECCCC0000000000000000000000000000000008000000000000000000C0A8010100000000", &match, sizeof(match));
}
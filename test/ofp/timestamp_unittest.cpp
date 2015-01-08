#include "ofp/unittest.h"
#include "ofp/timestamp.h"

using namespace ofp;

TEST(timestamp, testInvalid) {
    Timestamp t;
    EXPECT_FALSE(t.valid());
    EXPECT_EQ("0.000000000", t.toString());
}

TEST(timestamp, testValid) {
    Timestamp t{123456789, 123456789};
    EXPECT_TRUE(t.valid());
    EXPECT_EQ("123456789.123456789", t.toString());
}

TEST(timestamp, parseValid) {
    Timestamp t;

    EXPECT_TRUE(t.parse("123456789.123456789"));
    EXPECT_EQ(Timestamp(123456789, 123456789), t);
    EXPECT_EQ("123456789.123456789", t.toString());

    EXPECT_TRUE(t.parse("12.21"));
    EXPECT_EQ(Timestamp(12, 210000000), t);
    EXPECT_EQ("12.210000000", t.toString());

    EXPECT_TRUE(t.parse("0.0"));
    EXPECT_EQ(Timestamp(0, 0), t);
    EXPECT_EQ("0.000000000", t.toString());

    EXPECT_TRUE(t.parse("1.001"));
    EXPECT_EQ(Timestamp(1, 1000000), t);
    EXPECT_EQ("1.001000000", t.toString());

    EXPECT_TRUE(t.parse("1.001000"));
    EXPECT_EQ(Timestamp(1, 1000000), t);
    EXPECT_EQ("1.001000000", t.toString());

    EXPECT_TRUE(t.parse("1.0000010"));
    EXPECT_EQ(Timestamp(1, 1000), t);
    EXPECT_EQ("1.000001000", t.toString());

    EXPECT_TRUE(t.parse("1.000000001"));
    EXPECT_EQ(Timestamp(1, 1), t);
    EXPECT_EQ("1.000000001", t.toString());

    EXPECT_TRUE(t.parse("1.000000000"));
    EXPECT_EQ(Timestamp(1, 0), t);
    EXPECT_EQ("1.000000000", t.toString());

    EXPECT_TRUE(t.parse("1.100000000"));
    EXPECT_EQ(Timestamp(1, 100000000), t);
    EXPECT_EQ("1.100000000", t.toString());
}

TEST(timestamp, parseInvalid) {
    Timestamp t;

    EXPECT_FALSE(t.parse(""));
    EXPECT_FALSE(t.parse("0"));
    EXPECT_FALSE(t.parse("1"));
    EXPECT_FALSE(t.parse("123"));
    EXPECT_FALSE(t.parse(".0"));
    EXPECT_FALSE(t.parse("0."));
    EXPECT_FALSE(t.parse(" 0.0"));
    EXPECT_FALSE(t.parse("0.0 "));
    EXPECT_FALSE(t.parse("0.0t"));
    EXPECT_FALSE(t.parse("0. 0"));
    EXPECT_FALSE(t.parse("-10.1"));
    EXPECT_FALSE(t.parse("10.-1"));

    // too many digits after decimal pt
    EXPECT_FALSE(t.parse("1.0000000001"));
    EXPECT_FALSE(t.parse("1.0000000000"));
}
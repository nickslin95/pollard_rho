#include <gtest/gtest.h>

#include <long_arithmetic/long_int.hpp>

TEST(LongInt, SmallNumbers) {
    LongInt a("12");
    LongInt b("7");
    EXPECT_EQ(a + b, LongInt("19"));
    EXPECT_EQ(a - b, LongInt("5"));
    EXPECT_EQ(b - a, LongInt("-5"));
    EXPECT_EQ(a * b, LongInt("84"));
    EXPECT_EQ(a / b, LongInt("1"));
    EXPECT_EQ(a % b, LongInt("5"));
    EXPECT_EQ(b % a, LongInt("7"));
}

TEST(LongInt, BigNumbers) {
    LongInt a("15222514519776677");
    LongInt b("72057594037928017");

    EXPECT_EQ(a + b, LongInt("87280108557704694"));
    EXPECT_EQ(a - b, LongInt("-56835079518151340"));
    EXPECT_EQ(b - a, LongInt("56835079518151340"));
    EXPECT_EQ(a * b, LongInt("1096897771502532551423974641459509"));
    EXPECT_EQ(a / b, LongInt("0"));
    EXPECT_EQ(b / a, LongInt("4"));
    EXPECT_EQ(a % b, LongInt("15222514519776677"));
    EXPECT_EQ(b % a, LongInt("11167535958821309"));
}

TEST(LongInt, Comparison) {
    EXPECT_TRUE(LongInt("0") == 0);
    EXPECT_TRUE(LongInt("12") > 0);
    EXPECT_TRUE(LongInt("1") >= 0);
    EXPECT_TRUE(LongInt("-1") < 0);
    EXPECT_TRUE(LongInt("-4") <= 0);
    EXPECT_TRUE(LongInt("3") != 0);
}

TEST(LongInt, UnaryMinus) {
    LongInt n("19");
    LongInt m = -n;
    EXPECT_EQ(m, LongInt("-19"));
    EXPECT_EQ(n, LongInt("19"));
}

TEST(LongInt, AssignInt) {
    LongInt a = 1;
    LongInt b = -2;

    EXPECT_EQ(a + a, -b);
}
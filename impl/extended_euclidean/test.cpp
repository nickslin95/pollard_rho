#include <gtest/gtest.h>

#include <extended_euclidean/extended_euclidean.hpp>
#include <long_arithmetic/long_int.hpp>

#include <vector>

// ax + by = gcd(a, b)
template <class Int>
Int find_min_poisitve_solution(Int a, Int b, Int d) {
    Int x = 1;
    Int r = d - a * x;
    while (r % b != 0) {
        x += Int{1};
        r = d - a * x;
    }
    return x;
}

// a = bx (mod n)
template <class Int>
Int find_min_solution_in_ring(Int a, Int b, Int n) {
    Int x = 0;
    while ((a - b * x) % n != 0) {
        x += Int{1};
    }
    return x;
}

TEST(TestGCD, Simple) {
    EXPECT_EQ(GCD(5, 7), 1);
    EXPECT_EQ(GCD(12, 16), 4);
    EXPECT_EQ(GCD(99, 100), 1);
    EXPECT_EQ(GCD(64, 256), 64);
    EXPECT_EQ(GCD(225, 175), 25);
    EXPECT_EQ(GCD(-5, -7), 1);
}

TEST(TestGCD, CornerCases) {
    EXPECT_EQ(GCD(1, 0), 1);
    EXPECT_EQ(GCD(100, 0), 100);
    EXPECT_EQ(GCD(0, 100), 100);
    EXPECT_EQ(GCD(1, 1), 1);
    EXPECT_EQ(GCD(1, 100), 1);
    EXPECT_EQ(GCD(100, 1), 1);
    EXPECT_EQ(GCD(100, 100), 100);
    EXPECT_EQ(GCD(101, 97), 1);
    EXPECT_EQ(GCD(111, 37), 37);
}

TEST(TestGCD, BigNumbers) {
    EXPECT_EQ(GCD(103748594226, 857694234426), 6);
    EXPECT_EQ(GCD(7485923403429, 9475348532301), 3);
    EXPECT_EQ(GCD(211494358943284, 3457835823), 1);
}

TEST(TestExtendedEuclidean, Simple) {
    std::vector<std::pair<int64_t, int64_t>> samples = {
        {1, 1}, {12, 16}, {49, -54}, {-108, 96}, {527, 1258}};
    for (auto [a, b] : samples) {
        auto res = ExtendedEuclideanAlgorithm(a, b);
        int64_t d = std::get<0>(res);
        int64_t x = std::get<1>(res);
        int64_t y = std::get<2>(res);
        EXPECT_EQ(d, a * x + b * y);
        EXPECT_TRUE(x > 0);
    }
}

TEST(TestExtendedEuclidean, MinPositive) {
    std::vector<std::pair<int64_t, int64_t>> samples = {
        {1, 1}, {12, 16}, {49, -54}, {527, 1258}, {-108, 96}, {228, 1056}, {-5, -7}};
    for (auto [a, b] : samples) {
        auto res = ExtendedEuclideanAlgorithm<int64_t>(a, b);
        int64_t d = std::get<0>(res);
        int64_t x = std::get<1>(res);
        int64_t y = std::get<2>(res);
        EXPECT_EQ(d, a * x + b * y);
        EXPECT_EQ(x, find_min_poisitve_solution<int64_t>(a, b, d));
    }
}

TEST(TestSolveEquation, PrimeModulo) {
    std::vector<std::tuple<int64_t, int64_t, int64_t>> samples = {
        {3, 4, 7},    {1, 5, 11},   {6, 8, 13},   {11, 16, 23},
        {65, 65, 97}, {-11, 8, 13}, {11, -8, 13}, {-24, -31, 73}};
    for (auto [a, b, n] : samples) {
        int64_t res = SolveEquation(a, b, n);
        EXPECT_TRUE((b * res - a) % n == 0);
        EXPECT_LE(res, n);
        EXPECT_EQ(res, find_min_solution_in_ring<int64_t>(a, b, n));
    }
}

TEST(TestGCD, LongInt) {
    EXPECT_EQ(GCD(LongInt("23"), LongInt("19")), LongInt("1"));
    EXPECT_EQ(GCD(LongInt("225"), LongInt("175")), LongInt("25"));
    EXPECT_EQ(GCD(LongInt("72057594037928017"), LongInt("72057594089783747")), LongInt("1"));
}

TEST(TestExtendedEuclidean, LongInt) {
    std::vector<std::pair<LongInt, LongInt>> samples{
        {LongInt("23"), LongInt("-19")},
        {LongInt("-108"), LongInt("96")},
        {LongInt("72057594089783747"), LongInt("72057594037928017")}};
    for (auto [a, b] : samples) {
        auto res = ExtendedEuclideanAlgorithm<LongInt>(a, b);
        LongInt d = std::get<0>(res);
        LongInt x = std::get<1>(res);
        LongInt y = std::get<2>(res);
        EXPECT_EQ(d, a * x + b * y);
    }
}

TEST(TestSolveEquation, LongInt) {
    std::vector<std::tuple<LongInt, LongInt, LongInt>> samples = {
        {3, 4, 7},
        {6, 8, 13},
        {65, 65, 97},
        {-11, 8, 13},
        {-24, -31, 73},
        {490064540513, 170079681745, 1099513257113},
        {15222514519776677, 7110318376978981, 72057594089783747}};
    for (auto [a, b, n] : samples) {
        LongInt res = SolveEquation<LongInt>(a, b, n);
        EXPECT_TRUE((b * res - a) % n == 0);
    }
}
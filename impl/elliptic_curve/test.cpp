#include <gtest/gtest.h>

#include <elliptic_curve/ec_point.hpp>
#include <elliptic_curve/field.hpp>
#include <long_arithmetic/long_int.hpp>

#include <vector>

template <class Int>
ECPoint<Int> slow_power(ECPoint<Int> P, int64_t n) {
    ECPoint<Int> R;
    while (n) {
        R += P;
        n -= 1;
    }
    return R;
}

TEST(FiniteField, Simple) {
    std::vector<int64_t> primes{2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31};
    int64_t a = 97;
    int64_t b = 101;
    for (auto p : primes) {
        FieldElem<int64_t>::SetPrime(p);
        FieldElem A(a);
        FieldElem B(b);
        EXPECT_EQ(A + B, FieldElem(a + b));
        EXPECT_EQ(A * B, FieldElem(a * b));
        EXPECT_EQ(A - B, FieldElem(a - b));
        EXPECT_EQ(A / B, FieldElem(a * SolveEquation<int64_t>(1, b, p)));
    }
}

TEST(FiniteField, BigPrime) {
    LongInt a = 15222514519776677;
    LongInt b = 198915293914922;
    std::vector<LongInt> primes{1099511627791, 281474987479363, 72057594037928017};
    for (auto p : primes) {
        FieldElem<LongInt>::SetPrime(p);
        FieldElem A(a);
        FieldElem B(b);
        EXPECT_EQ(A + B, FieldElem(a + b));
        EXPECT_EQ(A * B, FieldElem(a * b));
        EXPECT_EQ(A - B, FieldElem(a - b));
        EXPECT_EQ(A / B, FieldElem(a * SolveEquation<LongInt>(1, b, p)));
    }
}

TEST(EllipticCurvePoint, SimpleOne) {
    EllipticCurve<int64_t> ec(7, 13, 97, 112);
    ECPoint<int64_t>::SetEllipticCurve(ec);
    ECPoint<int64_t> P(13, 19);
    ECPoint<int64_t> Q(92, 12);
    ECPoint<int64_t> R(17, 1);
    EXPECT_FALSE(P.IsNeutral());
    EXPECT_FALSE(Q.IsNeutral());
    EXPECT_EQ(P + Q, R);
    EXPECT_EQ(Q + P, R);
    EXPECT_EQ((P + Q) + R, P + (Q + R));

    ECPoint<int64_t> P_I(13, 78);
    ECPoint<int64_t> O;
    EXPECT_TRUE(O.IsNeutral());
    EXPECT_EQ(O + O, O);
    EXPECT_EQ(P_I, P.GetInverse());
    EXPECT_EQ(P + P_I, O);
    EXPECT_EQ(P + O, P);
    EXPECT_EQ(O + P, P);
}

TEST(EllipticCurvePoint, SimpleTwo) {
    EllipticCurve<int64_t> ec(345, 717, 1297, 1246);
    ECPoint<int64_t>::SetEllipticCurve(ec);
    ECPoint<int64_t> P(139, 53);
    ECPoint<int64_t> Q(418, 15);
    ECPoint<int64_t> R(747, 67);
    EXPECT_EQ(P + Q, R);
    EXPECT_EQ(Q + P, R);
    EXPECT_EQ((P + Q) + R, P + (Q + R));
    EXPECT_EQ(P.GetInverse() + Q.GetInverse(), (P + Q).GetInverse());

    ECPoint<int64_t> R_I = R.GetInverse();
    ECPoint<int64_t> O;
    EXPECT_EQ(R + R_I, O);
}

TEST(ECPointFastPower, Simple) {
    EllipticCurve<int64_t> ec(7, 13, 97, 112);
    ECPoint<int64_t>::SetEllipticCurve(ec);
    ECPoint<int64_t> P(13, 19);
    std::vector<int64_t> samples{1, 2, 3, 4, 5, 6, 9, 16, 22, 111, 112, 585};
    for (auto n : samples) {
        EXPECT_EQ(P.Power(n), slow_power<int64_t>(P, n));
    }
    EXPECT_EQ(P.Power(112), ECPoint<int64_t>());
}
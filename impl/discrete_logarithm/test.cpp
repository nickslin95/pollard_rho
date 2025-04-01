#include <gtest/gtest.h>

#include <discrete_logarithm/dl_finder.hpp>
#include <discrete_logarithm/tonelli_shanks.hpp>
#include <elliptic_curve/ec_point.hpp>

#include <vector>
#include <random>

class CyclicGroupElem {
public:
    CyclicGroupElem() = default;

    CyclicGroupElem(int val) : val_(((val % mod_) + mod_) % mod_) {
    }

    static void SetMod(int mod) {
        mod_ = mod;
    }

    CyclicGroupElem& operator+=(const CyclicGroupElem& other) {
        val_ += other.val_;
        val_ %= mod_;
        return *this;
    }

    CyclicGroupElem operator+(const CyclicGroupElem& other) const {
        CyclicGroupElem res = *this;
        res += other;
        return res;
    }

    CyclicGroupElem Power(int64_t n) const {
        CyclicGroupElem R(0);
        while (n) {
            R += *this;
            n -= 1;
        }
        return R;
    }

    bool operator==(const CyclicGroupElem& other) const {
        return val_ == other.val_;
    }

    int GetVal() const {
        return val_;
    }

private:
    static int mod_;
    int val_;
};

int CyclicGroupElem::mod_ = 0;

template <>
struct std::hash<CyclicGroupElem> {
    std::size_t operator()(const CyclicGroupElem& P) const {
        return std::hash<int>{}(P.GetVal());
    }
};

TEST(DL_CyclicGroup, Simple) {
    std::vector<int> primes{7, 13, 19, 31};
    for (auto n : primes) {
        CyclicGroupElem::SetMod(n);
        std::uniform_int_distribution<int> dist(1, n - 1);
        CyclicGroupElem alpha(11);
        CyclicGroupElem beta(29);
        DiscreteLogarithmFinder<CyclicGroupElem, int64_t> dl_finder(alpha, beta, n);
        auto res = dl_finder.Find();
        EXPECT_EQ(res, SolveEquation(beta.GetVal(), alpha.GetVal(), n));
    }
}

TEST(DL_CyclicGroup, Random) {
    std::mt19937 gen(42);
    std::vector<int> primes{5, 7, 13, 19, 23, 37, 73, 97, 101};
    for (auto n : primes) {
        CyclicGroupElem::SetMod(n);
        std::uniform_int_distribution<int> dist(1, n - 1);
        CyclicGroupElem alpha(dist(gen));
        CyclicGroupElem beta(dist(gen));
        while (alpha.GetVal() == 0 || beta.GetVal() == 0) {
            alpha = CyclicGroupElem(dist(gen));
            beta = CyclicGroupElem(dist(gen));
        }
        DiscreteLogarithmFinder<CyclicGroupElem, int64_t> dl_finder(alpha, beta, n);
        auto res = dl_finder.Find();
        EXPECT_EQ(res, SolveEquation(beta.GetVal(), alpha.GetVal(), n));
    }
}

TEST(DL_ECPoint, Simple) {
    EllipticCurve<int64_t> ec(43, 71, 149, 139);
    ECPoint<int64_t>::SetEllipticCurve(ec);
    ECPoint<int64_t> P(15, 51);
    ECPoint<int64_t> Q(79, 28);
    DiscreteLogarithmFinder<ECPoint<int64_t>, int64_t> dl_finder(P, Q, 139);
    auto res = dl_finder.Find();
    EXPECT_EQ(P.Power(res), Q);
}

TEST(TonelliShanks, Simple) {
    std::vector<int> primes{5, 7, 11, 13, 19, 29};
    int a = 101;
    for (auto p : primes) {
        int s = (a * a) % p;
        int r = TonelliShanks<int>(s, p);
        EXPECT_GE(r, 0);
        EXPECT_EQ((r * r) % p, s);
    }
}

static std::mt19937 gen(42);

template <class Int>
ECPoint<Int> GetRandomPoint(const EllipticCurve<Int>& ec, int64_t p) {
    std::uniform_int_distribution<int64_t> dist(1, p - 1);
    Int x, y;
    do {
        x = Int{dist(gen)};
        FieldElem<Int> X(x);
        FieldElem<Int> S = X * X * X + FieldElem(ec.A()) * X + FieldElem(ec.B());
        y = TonelliShanks(S.GetVal(), ec.Prime());
    } while (y == Int{-1});
    assert((y * y) % p == (x * x * x + ec.A() * x + ec.B()) % p);
    ECPoint P(x, y);
    return P;
}

TEST(DL_ECPoint, SmallPrimeRandom) {
    int64_t prime = 97;
    int64_t group_order = 101;
    EllipticCurve<int64_t> ec(26, 44, prime, group_order);
    ECPoint<int64_t>::SetEllipticCurve(ec);
    for (int i = 1; i < 1000; ++i) {
        ECPoint<int64_t> P = GetRandomPoint(ec, prime);
        ECPoint<int64_t> Q = GetRandomPoint(ec, prime);
        DiscreteLogarithmFinder<ECPoint<int64_t>, int64_t> dl_finder(P, Q, group_order);
        auto res = dl_finder.Find();
        ASSERT_EQ(P.Power(res), Q);
    }
}

TEST(DL_ECPoint, SmallPrimeLongInt) {
    int64_t prime = 97;
    int64_t group_order = 101;
    EllipticCurve<LongInt> ec(LongInt(26), LongInt(44), LongInt(prime), LongInt(group_order));
    ECPoint<LongInt>::SetEllipticCurve(ec);
    for (int i = 1; i < 1000; ++i) {
        ECPoint<LongInt> P = GetRandomPoint(ec, prime);
        ECPoint<LongInt> Q = GetRandomPoint(ec, prime);
        DiscreteLogarithmFinder<ECPoint<LongInt>, LongInt> dl_finder(P, Q, group_order);
        auto res = dl_finder.Find();
        ASSERT_EQ(P.Power(res), Q);
    }
}

TEST(DL_ECPoint, MediumPrimeLongInt) {
    int64_t prime = 7727;
    int64_t group_order = 7681;
    EllipticCurve<LongInt> ec(LongInt(149), LongInt(449), LongInt(prime), LongInt(group_order));
    ECPoint<LongInt>::SetEllipticCurve(ec);
    for (int i = 1; i < 1000; ++i) {
        ECPoint<LongInt> P = GetRandomPoint(ec, prime);
        ECPoint<LongInt> Q = GetRandomPoint(ec, prime);
        DiscreteLogarithmFinder<ECPoint<LongInt>, LongInt> dl_finder(P, Q, group_order);
        auto res = dl_finder.Find();
        ASSERT_EQ(P.Power(res), Q);
    }
}

TEST(DL_ECPoint, BigPrimeRandom) {
    int64_t prime = 1099511627791;
    int64_t group_order = 1099513257113;
    EllipticCurve<LongInt> ec(LongInt{490064540513}, LongInt{170079681745}, LongInt{prime},
                              LongInt{group_order});
    ECPoint<LongInt>::SetEllipticCurve(ec);
    ECPoint<LongInt> P = GetRandomPoint(ec, prime);
    ECPoint<LongInt> Q = GetRandomPoint(ec, prime);
    DiscreteLogarithmFinder<ECPoint<LongInt>, LongInt> dl_finder(P, Q, group_order);
    auto res = dl_finder.Find();
    EXPECT_EQ(P.Power(res), Q);
}
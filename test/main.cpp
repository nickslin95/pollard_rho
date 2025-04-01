#include <cstdint>
#include <cassert>
#include <iostream>
#include <random>

#include <discrete_logarithm/dl_finder.hpp>
#include <discrete_logarithm/tonelli_shanks.hpp>
#include <elliptic_curve/ec_point.hpp>
#include <elliptic_curve/field.hpp>
#include <long_arithmetic/long_int.hpp>

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
    assert((y * y) % Int{p} == (x * x * x + ec.A() * x + ec.B()) % Int{p});
    ECPoint P(x, y);
    return P;
}

void CheckDiscreteLogarithmOnEllipticCurve(int64_t p, int64_t a, int64_t b, int64_t q) {
    EllipticCurve<LongInt> ec(LongInt{a}, LongInt{b}, LongInt{p}, LongInt{q});
    ECPoint<LongInt>::SetEllipticCurve(ec);
    ECPoint<LongInt> P = GetRandomPoint<LongInt>(ec, p);
    ECPoint<LongInt> Q = GetRandomPoint<LongInt>(ec, p);
    DiscreteLogarithmFinder<ECPoint<LongInt>, LongInt> dl_finder(P, Q, q);
    auto res = dl_finder.Find();
    assert(P.Power(res) == Q);
}

int main() {
    CheckDiscreteLogarithmOnEllipticCurve(1099511627791, 490064540513, 170079681745, 1099513257113);
    std::cout << "First check passed!\n";
    CheckDiscreteLogarithmOnEllipticCurve(281474976710677, 187997080572537, 198915293914922, 281474987479363);
    std::cout << "Second check passed!\n";
    CheckDiscreteLogarithmOnEllipticCurve(72057594037928017, 15222514519776677, 7110318376978981, 72057594089783747);
    std::cout << "All checks passed!\n";
    return 0;
}
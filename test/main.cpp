/*
p -- характеристика поля, q -- порядок группы точек. p и  q -- простые
2^40: p = 1099511627791, a = 490064540513, b = 170079681745, q = 1099513257113
2^48: p = 281474976710677, a = 187997080572537, b = 198915293914922, q = 281474987479363
2^56: p = 72057594037928017, a = 15222514519776677, b = 7110318376978981, q = 72057594089783747
*/

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

ECPoint<LongInt> GetRandomPoint(const EllipticCurve<LongInt>& ec, int64_t p) {
    std::uniform_int_distribution<int64_t> dist(1, p - 1);
    LongInt x, y;
    do {
        x = LongInt{dist(gen)};
        FieldElem<LongInt> X(x);
        FieldElem<LongInt> S = X * X * X + FieldElem(ec.A()) * X + FieldElem(ec.B());
        LongInt y = TonelliShanks(S.GetVal(), ec.Prime());
    } while (y != LongInt{-1});
    ECPoint P(x, y);
    return P;
}

void CheckDiscreteLogarithmOnEllipticCurve(int64_t p, int64_t a, int64_t b, int64_t q) {
    EllipticCurve<LongInt> ec(LongInt{a}, LongInt{b}, LongInt{p}, LongInt{q});
    ECPoint<LongInt>::SetEllipticCurve(ec);
    ECPoint<LongInt> P = GetRandomPoint(ec, p);
    ECPoint<LongInt> Q = GetRandomPoint(ec, p);
    DiscreteLogarithmFinder<ECPoint<LongInt>> dl_finder(P, Q, q);
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
#pragma once

#include <cstdint>
#include <cassert>
#include <functional>

#include <elliptic_curve/field.hpp>
#include <long_arithmetic/long_int.hpp>

// elliptic curve y^2 = x^3 + ax + b over finite field F_p
template <class Int>
class EllipticCurve {
public:
    EllipticCurve() = default;
    EllipticCurve(Int a, Int b, Int p, Int q) : a_(a), b_(b), p_(p), q_(q) {
    }

    Int GroupOrder();
    Int A() const;
    Int B() const;
    Int Prime() const;

private:
    Int p_;
    Int a_;
    Int b_;
    Int q_;  // group order
};

template <class Int>
class ECPoint {
public:
    ECPoint() {
        neutral_ = true;
    }
    ECPoint(Int x, Int y);

    static void SetEllipticCurve(EllipticCurve<Int> ec);

    ECPoint& operator+=(const ECPoint& other);
    ECPoint operator+(const ECPoint& other) const;

    bool IsNeutral() const;
    ECPoint GetInverse() const;
    Int GroupOrder() const;
    ECPoint Power(Int n) const;

    bool operator==(const ECPoint& other) const;

    Int X() const;
    Int Y() const;

private:
    static EllipticCurve<Int> EC;

    bool neutral_;
    Int x_;
    Int y_;
};

template <>
struct std::hash<ECPoint<int64_t>> {
    std::size_t operator()(const ECPoint<int64_t>& P) const {
        auto h = std::hash<int64_t>{};
        return h(P.X()) ^ h(P.Y() << 1);
    }
};

template <>
struct std::hash<ECPoint<LongInt>> {
    std::size_t operator()(const ECPoint<LongInt>& P) const {
        auto h = std::hash<int64_t>{};
        return h(P.X().NarrowToInt()) ^ h(P.Y().NarrowToInt() << 1);
    }
};

template <class Int>
Int EllipticCurve<Int>::GroupOrder() {
    return q_;
}

template <class Int>
Int EllipticCurve<Int>::A() const {
    return a_;
}

template <class Int>
Int EllipticCurve<Int>::B() const {
    return b_;
}

template <class Int>
Int EllipticCurve<Int>::Prime() const {
    return p_;
}

template <class Int>
EllipticCurve<Int> ECPoint<Int>::EC;

template <class Int>
void ECPoint<Int>::SetEllipticCurve(EllipticCurve<Int> ec) {
    EC = ec;
    FieldElem<Int>::SetPrime(ec.Prime());
}

template <class Int>
ECPoint<Int>::ECPoint(Int x, Int y) {
    x_ = FieldElem<Int>(x).GetVal();
    y_ = FieldElem<Int>(y).GetVal();
    neutral_ = false;
}

template <class Int>
ECPoint<Int>& ECPoint<Int>::operator+=(const ECPoint& other) {
    if (other.neutral_) {
        return *this;
    } else if (neutral_) {
        *this = other;
        return *this;
    } else if (x_ == other.x_ && FieldElem<Int>(y_) == FieldElem<Int>(-other.y_)) {
        this->neutral_ = true;
        return *this;
    } else {
        FieldElem<Int> lambda;
        if (*this == other) {
            lambda = (FieldElem<Int>(3) * FieldElem<Int>(x_) * FieldElem<Int>(x_) +
                      FieldElem<Int>(EC.A())) /
                     (FieldElem<Int>(2) * FieldElem<Int>(y_));
        } else {
            lambda = (FieldElem<Int>(other.y_) - FieldElem<Int>(y_)) /
                     (FieldElem<Int>(other.x_) - FieldElem<Int>(x_));
        }
        FieldElem<Int> X = lambda * lambda - FieldElem<Int>(x_) - FieldElem<Int>(other.x_);
        FieldElem<Int> Y = lambda * (FieldElem<Int>(x_) - X) - FieldElem<Int>(y_);
        ECPoint point(X.GetVal(), Y.GetVal());
        *this = point;
        return *this;
    }
}

template <class Int>
ECPoint<Int> ECPoint<Int>::operator+(const ECPoint& other) const {
    ECPoint res = *this;
    res += other;
    return res;
}

template <class Int>
bool ECPoint<Int>::IsNeutral() const {
    return neutral_;
}

template <class Int>
ECPoint<Int> ECPoint<Int>::GetInverse() const {
    ECPoint inv = *this;
    inv.y_ = (FieldElem<Int>(0) - FieldElem<Int>(y_)).GetVal();
    return inv;
}

template <class Int>
Int ECPoint<Int>::GroupOrder() const {
    return EC.GroupOrder();
}

template <class Int>
bool ECPoint<Int>::operator==(const ECPoint& other) const {
    if (neutral_ || other.neutral_) {
        return neutral_ && other.neutral_;
    }
    return (x_ == other.x_) && (y_ == other.y_);
}

template <class Int>
Int ECPoint<Int>::X() const {
    return x_;
}

template <class Int>
Int ECPoint<Int>::Y() const {
    return y_;
}

template <class Int>
ECPoint<Int> ECPoint<Int>::Power(Int n) const {
    ECPoint Q = *this;
    ECPoint R;
    while (n > 0) {
        if (n % Int{2} == Int{1}) {
            R += Q;
        }
        Q += Q;
        n /= Int{2};
    }
    return R;
}
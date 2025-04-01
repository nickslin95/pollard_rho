#pragma once

#include <cstdint>

#include <extended_euclidean/extended_euclidean.hpp>

template <class Int>
class FieldElem {
public:
    FieldElem() = default;
    FieldElem(Int val);

    static void SetPrime(Int prime);

    FieldElem& operator+=(const FieldElem& other);
    FieldElem& operator-=(const FieldElem& other);
    FieldElem& operator*=(const FieldElem& other);
    FieldElem& operator/=(const FieldElem& other);

    FieldElem operator+(const FieldElem& other) const;
    FieldElem operator-(const FieldElem& other) const;
    FieldElem operator*(const FieldElem& other) const;
    FieldElem operator/(const FieldElem& other) const;

    bool operator==(const FieldElem& other) const;
    Int GetVal() const;

private:
    static Int P;
    Int val_;
};

template <class Int>
Int FieldElem<Int>::P;

template <class Int>
FieldElem<Int>::FieldElem(Int val) {
    val_ = ((val % P) + P) % P;
}

template <class Int>
void FieldElem<Int>::SetPrime(Int prime) {
    P = prime;
}

template <class Int>
FieldElem<Int>& FieldElem<Int>::operator+=(const FieldElem& other) {
    val_ += other.val_;
    val_ %= P;
    return *this;
}

template <class Int>
FieldElem<Int>& FieldElem<Int>::operator-=(const FieldElem& other) {
    val_ += (P - other.val_);
    val_ %= P;
    return *this;
}

template <class Int>
FieldElem<Int>& FieldElem<Int>::operator*=(const FieldElem& other) {
    val_ *= other.val_;
    val_ %= P;
    return *this;
}

template <class Int>
FieldElem<Int>& FieldElem<Int>::operator/=(const FieldElem& other) {
    FieldElem inv(SolveEquation<Int>(1, other.val_, P));
    *this *= inv;
    return *this;
}

template <class Int>
FieldElem<Int> FieldElem<Int>::operator+(const FieldElem& other) const {
    FieldElem res = *this;
    res += other;
    return res;
}

template <class Int>
FieldElem<Int> FieldElem<Int>::operator-(const FieldElem& other) const {
    FieldElem res = *this;
    res -= other;
    return res;
}

template <class Int>
FieldElem<Int> FieldElem<Int>::operator*(const FieldElem& other) const {
    FieldElem res = *this;
    res *= other;
    return res;
}

template <class Int>
FieldElem<Int> FieldElem<Int>::operator/(const FieldElem& other) const {
    FieldElem res = *this;
    res /= other;
    return res;
}

template <class Int>
bool FieldElem<Int>::operator==(const FieldElem& other) const {
    return val_ == other.val_;
}

template <class Int>
Int FieldElem<Int>::GetVal() const {
    return val_;
}
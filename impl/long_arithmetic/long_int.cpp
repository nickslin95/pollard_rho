#include "long_int.hpp"

LongInt& LongInt::operator+=(const LongInt& other) {
    *this = (*this + other);
    return *this;
}

LongInt& LongInt::operator-=(const LongInt& other) {
    *this = (*this - other);
    return *this;
}

LongInt& LongInt::operator*=(const LongInt& other) {
    *this = (*this * other);
    return *this;
}

LongInt& LongInt::operator/=(const LongInt& other) {
    *this = (*this / other);
    return *this;
}

LongInt& LongInt::operator%=(const LongInt& other) {
    *this = (*this % other);
    return *this;
}

LongInt LongInt::operator+(const LongInt& other) const {
    LongInt res;
    mpz_add(res.val_, val_, other.val_);
    return res;
}

LongInt LongInt::operator-(const LongInt& other) const {
    LongInt res;
    mpz_sub(res.val_, val_, other.val_);
    return res;
}

LongInt LongInt::operator*(const LongInt& other) const {
    LongInt res;
    mpz_mul(res.val_, val_, other.val_);
    return res;
}
LongInt LongInt::operator/(const LongInt& other) const {
    LongInt res;
    mpz_fdiv_q(res.val_, val_, other.val_);
    return res;
}

LongInt LongInt::operator-() const {
    LongInt res;
    mpz_sub(res.val_, res.val_, val_);
    return res;
}

LongInt LongInt::operator%(const LongInt& other) const {
    LongInt res;
    mpz_mod(res.val_, val_, other.val_);
    return res;
}

bool LongInt::operator==(const LongInt& other) const {
    return (mpz_cmp(val_, other.val_) == 0);
}

bool LongInt::operator!=(const LongInt& other) const {
    return !(this->operator==(other));
}

bool LongInt::operator<(const LongInt& other) const {
    return (mpz_cmp(val_, other.val_) < 0);
}

bool LongInt::operator==(const int& v) const {
    return mpz_cmp_ui(val_, v) == 0;
}

bool LongInt::operator!=(const int& v) const {
    return !this->operator==(v);
}

bool LongInt::operator<(const int& v) const {
    return mpz_cmp_ui(val_, v) < 0;
}

bool LongInt::operator<=(const int& v) const {
    return this->operator<(v) || this->operator==(v);
}

bool LongInt::operator>(const int& v) const {
    return !this->operator<=(v);
}

bool LongInt::operator>=(const int& v) const {
    return !this->operator<(v);
}

LongInt& LongInt::operator=(int64_t v) {
    mpz_init_set_si(val_, v);
    return *this;
}
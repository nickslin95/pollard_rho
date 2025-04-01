#pragma once

#include <gmp.h>

#include <string>

class LongInt {
public:
    LongInt() {
        mpz_init(val_);
    }

    LongInt(const std::string& str) {
        mpz_init(val_);
        mpz_set_str(val_, str.c_str(), 10);
    }

    LongInt(int64_t v) {
        mpz_init_set_si(val_, v);
    }

    LongInt& operator=(int64_t v);

    LongInt(const LongInt& other) {
        mpz_init(val_);
        mpz_init_set(val_, other.val_);
    }

    LongInt& operator=(const LongInt& other) {
        mpz_init(val_);
        mpz_init_set(val_, other.val_);
        return *this;
    }

    LongInt(LongInt&& other) {
        mpz_init(val_);
        mpz_init_set(val_, other.val_);
    }

    LongInt& operator=(LongInt&& other) {
        mpz_init(val_);
        mpz_init_set(val_, other.val_);
        return *this;
    }

    ~LongInt() {
        mpz_clear(val_);
    }

    int64_t NarrowToInt() const {
        return mpz_get_si(val_);
    }

    LongInt operator-() const;
    LongInt& operator+=(const LongInt& other);
    LongInt& operator-=(const LongInt& other);
    LongInt& operator*=(const LongInt& other);
    LongInt& operator/=(const LongInt& other);
    LongInt& operator%=(const LongInt& other);

    LongInt operator+(const LongInt& other) const;
    LongInt operator-(const LongInt& other) const;
    LongInt operator*(const LongInt& other) const;
    LongInt operator/(const LongInt& other) const;
    LongInt operator%(const LongInt& other) const;

    bool operator==(const LongInt& other) const;
    bool operator!=(const LongInt& other) const;
    bool operator<(const LongInt& other) const;
    bool operator==(const int& v) const;
    bool operator!=(const int& v) const;
    bool operator<(const int& v) const;
    bool operator<=(const int& v) const;
    bool operator>(const int& v) const;
    bool operator>=(const int& v) const;

private:
    mpz_t val_;
};
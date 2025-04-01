#pragma once

#include <cstdint>
#include <tuple>

template <class Int>
Int GCD(Int a, Int b) {
    if (b < 0) {
        b = -b;
    }
    if (a < 0) {
        a = -a;
    }
    while (b > 0) {
        Int q = a / b;
        Int r = a - q * b;
        a = b;
        b = r;
    }
    return a;
}

// find gcd(a, b) and soultion of the equation ax + by = gcd(a, b)
template <class Int>
std::tuple<Int, Int, Int> ExtendedEuclideanAlgorithm(Int a, Int b) {
    Int u = 1, g = a, x = 0, y = b;
    while (y != 0) {
        Int q = g / y;
        Int t = g - q * y;
        Int s = u - q * x;
        u = x;
        g = y;
        x = s;
        y = t;
    }
    Int v = (g - a * u) / b;

    if (u > 0) {
        if (b / g > 0) {
            Int div = b / g;
            Int quot = u / div;
            u -= quot * (b / g);
            v += quot * (a / g);
        } else {
            Int div = -(b / g);
            Int quot = u / div;
            u += quot * (b / g);
            v -= quot * (a / g);
        }
    } else {
        if (b / g > 0) {
            Int div = b / g;
            Int quot = (-u) / div;
            quot += Int{1};
            u += quot * (b / g);
            v -= quot * (a / g);
        } else {
            Int div = -(b / g);
            Int quot = (-u) / div;
            quot += Int{1};
            u -= quot * (b / g);
            v += quot * (a / g);
        }
    }
    return std::make_tuple(g, u, v);
}

// find solution of A = Bx (mod n) in ring Z/nZ
template <class Int>
Int SolveEquation(Int A, Int B, Int n) {
    auto res = ExtendedEuclideanAlgorithm(B, n);
    Int gcd = std::get<0>(res);
    Int u = std::get<1>(res);
    Int ans = ((u * (A / gcd)) % n);
    if (n < 0) {
        n = -n;
    }
    return (ans + n) % n;
}
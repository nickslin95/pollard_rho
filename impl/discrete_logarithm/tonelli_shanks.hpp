#pragma once

template <class Int>
Int ModExp(Int base, Int exp, Int mod) {
    Int result = 1;
    base = base % mod;
    while (exp > 0) {
        if (exp % Int{2} == Int{1})
            result = (result * base) % mod;
        exp = exp / Int{2};   
        base = (base * base) % mod;  
    }
    return result;
}

template <class Int>
Int LegendreSymbol(Int a, Int p) {
    Int ls = ModExp(a, (p - Int{1}) / Int{2}, p);
    return (ls == p - Int{1}) ? Int{-1} : ls;
}

template <class Int>
Int TonelliShanks(Int a, Int p) {
    if (LegendreSymbol(a, p) != Int{1})
        return Int{-1}; 

    if (p % Int{4} == Int{3})
        return ModExp(a, (p + 1) / Int{4}, p);

    Int s = 0;
    Int q = p - Int{1};
    while (q % Int{2} == 0) {
        q /= Int{2};
        s += Int{1};
    }

    Int z = 2;
    while (LegendreSymbol(z, p) != Int{-1}) {
        z += Int{1};
    }

    Int m = s;
    Int c = ModExp(z, q, p);
    Int t = ModExp(a, q, p);
    Int r = ModExp(a, (q + 1) / Int{2}, p);

    while (t != 0 && t != 1) {
        Int i = 0;
        Int temp = t;
        while (temp != 1 && i < m) {
            temp = (temp * temp) % p;
            i += Int{1};
        }

        Int counter = m - i - Int{1};
        Int power = 1;
        while (counter > 0) {
            counter -= Int{1};
            power *= Int{2};
        }

        Int b = ModExp(c, power, p);
        r = (r * b) % p;
        c = (b * b) % p;
        t = (t * c) % p;
        m = i;
    }

    return r;
}
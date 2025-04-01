#pragma once

#include <functional>
#include <random>

#include <extended_euclidean/extended_euclidean.hpp>

// GroupElem requires operator+(), operator==(), std::hash, Power()

template <class GroupElem, class Int>
class DiscreteLogarithmFinder {
public:
    DiscreteLogarithmFinder(GroupElem alpha, GroupElem beta, int64_t order)
        : alpha_{alpha}, beta_{beta}, group_order_{order}, init_order_{order} {
    }

    Int Find() const {
        Triple slow = {GroupElem(), 0, 0};
        while (true) {
            Triple fast = slow;
            while (true) {
                slow = Next(slow);
                fast = Next(Next(fast));
                if (slow.x == fast.x) {
                    Int A = slow.a - fast.a;
                    Int B = fast.b - slow.b;
                    if (B == 0) {
                        slow = GetRandomTriple();
                        break;
                    }
                    // find solution of A = Bx (mod n)
                    Int res = SolveEquation<Int>(A, B, group_order_);
                    return res;
                }
            }
        }
    }

private:
    static std::mt19937 gen_;

    // invariant: x = alpha^a * beta^b
    struct Triple {
        GroupElem x;
        Int a;
        Int b;
    };

    Triple GetRandomTriple() const {
        std::uniform_int_distribution<int64_t> dist(0, init_order_ - 1);
        Triple res;
        res.a = Int{dist(gen_)};
        res.b = Int{dist(gen_)};
        res.x = alpha_.Power(res.a) + beta_.Power(res.b);
        return res;
    }

    Triple Next(Triple t) const {
        auto hash = std::hash<GroupElem>{};
        if (hash(t.x) % 3 == 0) {
            return Triple{t.x + beta_, t.a, (t.b + Int{1}) % group_order_};
        } else if (hash(t.x) % 3 == 1) {
            return Triple{t.x + t.x, (t.a * Int{2}) % group_order_, (t.b * Int{2}) % group_order_};
        } else {
            return Triple{t.x + alpha_, (t.a + Int{1}) % group_order_, t.b};
        }
    }

    GroupElem alpha_;
    GroupElem beta_;
    Int group_order_;
    int64_t init_order_;
};

template <class GroupElem, class Int>
std::mt19937 DiscreteLogarithmFinder<GroupElem, Int>::gen_(42);
// Copyright Louis Dionne 2014
// Distributed under the Boost Software License, Version 1.0.

#include <boost/hana/integral.hpp>
using namespace boost::hana;


template <typename T>
void assert_positive(T i) {
    static_assert(value(i) > 0, "");
}

int main() {
    auto i = integral<int, 2>;
    assert_positive(i);
}

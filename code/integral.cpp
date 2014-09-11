// Copyright Louis Dionne 2014
// Distributed under the Boost Software License, Version 1.0.

#include <boost/hana/detail/assert.hpp>
#include <boost/hana/integral.hpp>

#include <type_traits>
using namespace boost::hana;


template <typename I>
void assert_positive(I) {
    static_assert(I::value > 0, "");
}

int main() {
    auto i = std::integral_constant<int, 2>{};
    assert_positive(i);

    BOOST_HANA_CONSTANT_ASSERT(int_<1> + int_<2> == int_<3>);
    assert_positive(int_<5> - int_<2>);
}

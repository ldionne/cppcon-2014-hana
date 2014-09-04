// Copyright Louis Dionne 2014
// Distributed under the Boost Software License, Version 1.0.

#include <boost/hana/detail/assert.hpp>
#include <boost/hana/ext/std/integral_constant.hpp>
#include <boost/hana/functional/on.hpp>
#include <boost/hana/orderable/orderable.hpp>
#include <boost/hana/tuple.hpp>
#include <boost/hana/type.hpp>

#include <tuple>
#include <type_traits>
using namespace boost::hana;


template <typename ...T>
using smallest = decltype(
    minimum_by(ordering(sizeof_), tuple(type<T>...))
);

template <typename ...T>
auto optimize_alignment = sort_by(
    greater ^on^ trait<std::alignment_of>, tuple(type<T>...)
);


template <int i>
struct storage { char s[i]; };

int main() {
    // smallest
    {
        static_assert(std::is_same<
            smallest<storage<3>, storage<1>, storage<2>>::type,
            storage<1>
        >::value, "");
    }


    // optimize_alignment
    {
        BOOST_HANA_CONSTANT_ASSERT(
            optimize_alignment<int, char, long double>
            ==
            tuple(type<long double>, type<int>, type<char>)
        );

        using OptimalTuple = decltype(
            unpack(
                optimize_alignment<int, char, long double>,
                template_<std::tuple>
            )
        )::type;

        static_assert(std::is_same<
            OptimalTuple, std::tuple<long double, int, char>
        >::value, "");
    }
}

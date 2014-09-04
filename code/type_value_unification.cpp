// Copyright Louis Dionne 2014
// Distributed under the Boost Software License, Version 1.0.

#include <boost/hana/type.hpp>

#include <boost/mpl/quote.hpp>
#include <tuple>
#include <type_traits>
using namespace boost::hana;


int main() {
    // metafunctions
    {
        auto int_pointer = metafunction<std::add_pointer>(type<int>);

        static_assert(std::is_same<
            decltype(int_pointer)::type, int*
        >::value, "");
    }


    // metafunction classes
    {
        using add_pointer = boost::mpl::quote1<std::add_pointer>;
        auto int_pointer = metafunction_class<add_pointer>(type<int>);

        static_assert(std::is_same<
            decltype(int_pointer)::type, int*
        >::value, "");
    }


    // templates
    {
        auto some_tuple = template_<std::tuple>(type<int>, type<char>);

        static_assert(std::is_same<
            decltype(some_tuple)::type,
            std::tuple<int, char>
        >::value, "");
    }
}

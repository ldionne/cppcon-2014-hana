// Copyright Louis Dionne 2014
// Distributed under the Boost Software License, Version 1.0.

#include <boost/hana.hpp>

#include <boost/hana/ext/std/integral_constant.hpp>
#include <boost/hana/ext/std/tuple.hpp>

#include <cassert>
#include <iostream>
#include <sstream>
#include <string>
#include <type_traits>
using namespace boost::hana;


int main() {
    //////////////////////////////////////////////////////////////////////////
    // Foldable
    //////////////////////////////////////////////////////////////////////////
    {
        auto is_even = [](auto x) {
            return x % int_<2> == int_<0>;
        };

        assert(unpack(tuple(1, '2', 3.3, std::string{"foo"}), [](auto ...xs) {
            return size_t<sizeof...(xs)>;
        }) == size_t<4>);

        assert(count(range(int_<0>, int_<10>), is_even) == int_<5>);
        assert(foldl(range(int_<1>, int_<5>), int_<1>, _ * _) == int_<24>);

        assert(sum(just(1)) == 1);
        assert(sum(nothing) == int_<0>);

        assert(length(nothing) == int_<0>);
        assert(length(just(nullptr)) == int_<1>);
    }

    //////////////////////////////////////////////////////////////////////////
    // Iterable
    //////////////////////////////////////////////////////////////////////////
    {
        auto xs = tuple(1, '2', 3.3, std::string{"foo"});
        auto r = range_c<int, 10, 20>;

        assert(head(xs) == 1);
        assert(last(xs) == "foo");
        assert(at_c<2>(xs) == 3.3);
        assert(drop_until(xs, trait_<std::is_floating_point>) ==
            tuple(3.3, "foo")
        );

        assert((tail(r) == range_c<int, 11, 20>));
        assert((drop_c<3>(r) == range_c<int, 13, 20>));
        assert((drop_while(r, _ < int_<15>) == range_c<int, 15, 20>));
    }

    //////////////////////////////////////////////////////////////////////////
    // Searchable
    //////////////////////////////////////////////////////////////////////////
    {
        auto xs = tuple(1, '2', 3.3, std::string{"foo"});
        auto ys = map(
            pair(type<float>, 4),
            pair(int_<1>, std::string{"foo"})
        );
        auto zs = set(int_<1>, type<char>, 3.3, std::string{"foo"});

        assert(find(xs, trait_<std::is_floating_point>) == just(3.3));
        assert(subset(tuple('2', "foo"), xs));

        assert(lookup(ys, type<float>) == just(4));
        assert(lookup(ys, type<char>) == nothing);
        assert(elem(ys, int_<1>));

        assert(any(zs, trait_<std::is_floating_point>));
        assert(3.3 ^in^ zs); // infix application :-)
    }

    //////////////////////////////////////////////////////////////////////////
    // List
    //////////////////////////////////////////////////////////////////////////
    {
        auto xs = tuple(1, '2', 3.3, std::string{"foo"});

        assert(xs == concat(tuple(1, '2'), tuple(3.3, "foo")));

        assert(reverse(xs) == tuple("foo", 3.3, '2', 1));

        assert((slice_c<1, 3>(xs) == tuple('2', 3.3)));

        assert(take_until(xs, trait_<std::is_floating_point>) ==
            tuple(1, '2')
        );
    }

    //////////////////////////////////////////////////////////////////////////
    // Functor
    //////////////////////////////////////////////////////////////////////////
    {
        auto xs = tuple(1, 3.3, std::string{"foo"});
        auto twice = [](auto x) { return x + x; };

        assert(fmap(xs, twice) == tuple(2, 6.6, "foofoo"));

        assert(adjust(xs, trait_<std::is_floating_point>, twice) ==
            tuple(1, 6.6, "foo")
        );

        assert(replace(just(1.3), trait_<std::is_floating_point>, 'x') ==
            just('x')
        );

        assert(fill(just(1.3), 'x') == just('x'));
        assert(fill(nothing, 'x') == nothing);
    }

    //////////////////////////////////////////////////////////////////////////
    // Comparable
    //////////////////////////////////////////////////////////////////////////
    {
        // Any two Lists
        assert(equal(tuple(1, '2', 3.3), std::make_tuple(1, '2', 3.3)));

        // Maybes
        assert(equal(just(1), just(1)));
        assert(not_equal(just(1), nothing));

        // EqualityComparable types
        assert(equal(std::string{"foo"}, std::string{"foo"}));

        // and about everything else provided by Hana
        assert(equal(set(1, '2', 3.3), set(3.3, 1, '2')));
    }

    //////////////////////////////////////////////////////////////////////////
    // Orderable
    //////////////////////////////////////////////////////////////////////////
    {
        // IntegralConstants
        assert(int_<3> < int_<5>);
        assert((
            std::integral_constant<long, 10>{} >=
            std::integral_constant<unsigned, 3>{}
        ));

        // LessThanComparable types
        assert(less(1, 3));
        assert(greater(std::string{"foo"}, std::string{"bar"}));

        // More will be provided when I get the time :-)
    }

    //////////////////////////////////////////////////////////////////////////
    // Monoid, Group, Ring, IntegralDomain
    //////////////////////////////////////////////////////////////////////////
    {
        // Monoid
        assert(zero<int> == 0);
        assert(zero<Integral> == int_<0>);
        assert(int_<3> + int_<5> == int_<8>);

        // Group
        assert(int_<3> - int_<7> == int_<-4>);
        assert(-int_<3> == int_<-3>);

        // Ring
        assert(one<long> == 1l);
        assert(one<Integral> == int_<1>);
        assert(int_<2> * int_<4> == int_<8>);

        // IntegralDomain
        assert(int_<6> % int_<3> == int_<0>);
        assert(int_<6> / int_<3> == int_<2>);
    }
}

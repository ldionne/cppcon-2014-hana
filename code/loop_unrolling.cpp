// Copyright Louis Dionne 2014
// Distributed under the Boost Software License, Version 1.0.

#include <boost/hana/integral.hpp>
#include <boost/hana/range.hpp>

#include <utility>


auto unroll = [](auto n, auto&& f) {
    boost::hana::for_each(boost::hana::range(boost::hana::int_<0>, n),
        [f(std::forward<decltype(f)>(f))](auto i) { f(); }
    );
};

void f(); // can't be inlined

int main() {
    unroll(boost::hana::int_<10>, f);
}

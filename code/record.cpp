// Copyright Louis Dionne 2014
// Distributed under the Boost Software License, Version 1.0.

#define BOOST_PP_VARIADICS 1
#include <boost/hana/foreign/comparable.hpp>
#include <boost/hana/record/macros.hpp>
#include <boost/hana/searchable/record_mcd.hpp>
#include <boost/hana/type.hpp>

#include <cassert>
#include <string>
using namespace boost::hana;


auto name = decltype_([]{});
auto age = decltype_([]{});

struct Person {
    BOOST_HANA_DEFINE_RECORD_INTRUSIVE(Person,
        (::name, (std::string, name)),
        (::age, (int, age))
    );
};

int main() {
    Person louis{"Louis", 22};
    assert(lookup(louis, name) == just("Louis"));
}

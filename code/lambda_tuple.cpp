
#include "lambda_tuple.hpp"
#include <cassert>


using namespace lambda;

int main() {
    // tuple
    tuple<int, char, double> ts{1, '2', 3.3};

    // make_tuple
    make_tuple();
    make_tuple(1, '2', 3.3);

    // get
    assert(get<0>(ts) == 1);
    assert(get<1>(ts) == '2');
    assert(get<2>(ts) == 3.3);

    // tuple_transform
    auto us = tuple_transform(ts, [](auto x) { return x + 1; });
    assert(get<0>(us) == 1 + 1);
    assert(get<1>(us) == '2' + 1);
    assert(get<2>(us) == 3.3 + 1);

    // apply
    auto sum = [](auto x, auto y, auto z) { return x + y + z; };
    assert(apply(sum, make_tuple(1, 2, 3)) == 1 + 2 + 3);

    // tuple_cat
    auto cat = tuple_cat(make_tuple(1, '2'), make_tuple(3.3, nullptr, 5));
    assert(
        get<0>(cat) == 1 &&
        get<1>(cat) == '2' &&
        get<2>(cat) == 3.3 &&
        get<3>(cat) == nullptr &&
        get<4>(cat) == 5
    );
}

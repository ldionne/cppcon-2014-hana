
#include "lambda_tuple.hpp"
#include <cassert>


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

    // transform
    auto us = transform(ts, [](auto x) { return x + 1; });
    assert(get<0>(us) == 1 + 1);
    assert(get<1>(us) == '2' + 1);
    assert(get<2>(us) == 3.3 + 1);

    // apply
    auto sum = [](auto x, auto y, auto z) { return x + y + z; };
    assert(apply(sum, make_tuple(1, 2, 3)) == 1 + 2 + 3);
}

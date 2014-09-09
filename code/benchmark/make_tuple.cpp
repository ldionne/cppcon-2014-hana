// Copyright Louis Dionne 2014
// Distributed under the Boost Software License, Version 1.0.

#include "../<%=technique%>.hpp"


template <int i>
struct x { };

int main() {
    auto xs = make_tuple(
        <%= (1..n_elements).to_a.map{ |i| "x<#{i}>{}" }.join(',') %>
    );
}

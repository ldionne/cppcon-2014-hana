// Copyright Louis Dionne 2014
// Distributed under the Boost Software License, Version 1.0.

#include "../<%=technique%>.hpp"


template <int i>
struct x { };

int main() {
    auto xs = make_tuple(
        // Make sure we use a non-empty tuple.
        <%= (1..n_elements+1).to_a.map{ |i| "x<#{i}>{}" }.join(',') %>
    );

    get< <%=n_elements/2%> >(xs);
}

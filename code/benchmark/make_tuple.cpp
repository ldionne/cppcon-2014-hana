
#include "../lambda_tuple.hpp"
#include "../std_tuple.hpp"

<%= setup %>


template <int i>
struct x { };

int main() {
    auto xs = benchmark::make_tuple(
        <%= (1..n_elements).to_a.map{ |i| "x<#{i}>{}" }.join(',') %>
    );
}

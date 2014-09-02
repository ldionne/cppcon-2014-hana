
#include "../lambda_tuple.hpp"
#include "../std_tuple.hpp"

<%= setup %>


template <int i>
struct x { };

int main() {
    other::make_tuple(
        <%= (1..n_elements).to_a.map{ |i| "x<#{i}>{}" }.join(',') %>
    );

    auto xs = benchmark::make_tuple(
        <%= (1..n_elements).to_a.map{ |i| "x<#{i}>{}" }.join(',') %>
    );

    <% 10.times do %>
        benchmark::apply([](auto ...x) { }, xs);
    <% end %>
}

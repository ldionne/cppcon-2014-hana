
#include "../lambda_tuple.hpp"
#include "../std_tuple.hpp"

<%= setup %>


template <int i>
struct x { };

int main() {
    <% if no_bias %>
        other::make_tuple(
            <%= (1..n_elements).to_a.map{ |i| "x<#{i}>{}" }.join(',') %>
        );
    <% end %>

    auto xs = benchmark::make_tuple(
        <%= (1..n_elements).to_a.map{ |i| "x<#{i}>{}" }.join(',') %>
    );

    <% 10.times do %>
        benchmark::tuple_for_each(xs, [](auto x) {

        });
    <% end %>
}

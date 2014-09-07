
#include "../fusion_vector.hpp"
#include "../lambda_tuple.hpp"
#include "../std_tuple.hpp"

<%= setup %>


template <int i>
struct x { };

template <int i>
struct y { };

int main() {
    <% if no_bias %>
        other1::make_tuple(
            <%= (1..n_elements).to_a.map{ |i| "x<#{i}>{}" }.join(',') %>
        );

        other1::make_tuple(
            <%= (1..n_elements).to_a.map{ |i| "y<#{i}>{}" }.join(',') %>
        );


        other2::make_tuple(
            <%= (1..n_elements).to_a.map{ |i| "x<#{i}>{}" }.join(',') %>
        );

        other2::make_tuple(
            <%= (1..n_elements).to_a.map{ |i| "y<#{i}>{}" }.join(',') %>
        );
    <% end %>


    auto xs = benchmark::make_tuple(
        <%= (1..n_elements).to_a.map{ |i| "x<#{i}>{}" }.join(',') %>
    );

    auto ys = benchmark::make_tuple(
        <%= (1..n_elements).to_a.map{ |i| "y<#{i}>{}" }.join(',') %>
    );

    benchmark::tuple_cat(xs, ys);
}

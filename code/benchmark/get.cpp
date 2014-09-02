
#include "../lambda_tuple.hpp"
#include "../std_tuple.hpp"

<%= setup %>


template <int i>
struct x { };

int main() {
    other::make_tuple(
        <%= (1..n_elements+1).to_a.map{ |i| "x<#{i}>{}" }.join(',') %>
    );

    auto xs = benchmark::make_tuple(
        // Make sure we use a non-empty tuple.
        <%= (1..n_elements+1).to_a.map{ |i| "x<#{i}>{}" }.join(',') %>
    );

    <% (0..n_elements).each do |i| %>
        benchmark::get< <%=i%> >(xs);
    <% end %>
}

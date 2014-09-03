
#include "../lambda_tuple.hpp"
#include "../std_tuple.hpp"

<%= setup %>


template <int i>
struct x { };

int main() {
    <% if no_bias %>
        other::make_tuple(
            <%= (1..n_elements+1).to_a.map{ |i| "x<#{i}>{}" }.join(',') %>
        );
    <% end %>

    auto xs = benchmark::make_tuple(
        // Make sure we use a non-empty tuple.
        <%= (1..n_elements+1).to_a.map{ |i| "x<#{i}>{}" }.join(',') %>
    );

    benchmark::get< <%=n_elements/2%> >(xs);
}

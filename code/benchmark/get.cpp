
#include "../<%=technique%>.hpp"


template <int i>
struct x { };

int main() {
    auto xs = make_tuple(
        // Make sure we use a non-empty tuple.
        <%= (1..n_elements+1).to_a.map{ |i| "x<#{i}>{}" }.join(',') %>
    );
    // Fetch in the middle of the sequence.
    constexpr auto n = <%= (n_elements+1) / 2 %>;
    get<n>(xs);
}

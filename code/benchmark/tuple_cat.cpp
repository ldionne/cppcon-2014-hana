
#include "../<%=technique%>.hpp"


template <int i>
struct x { };

template <int i>
struct y { };

int main() {
    auto xs = make_tuple(
        <%= (1..n_elements).to_a.map{ |i| "x<#{i}>{}" }.join(',') %>
    );

    auto ys = make_tuple(
        <%= (1..n_elements).to_a.map{ |i| "y<#{i}>{}" }.join(',') %>
    );

    tuple_cat(xs, ys);
}

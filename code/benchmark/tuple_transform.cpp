
#include "../<%=technique%>.hpp"


template <int i>
struct x { };

int main() {
    auto xs = make_tuple(
        <%= (1..n_elements).to_a.map{ |i| "x<#{i}>{}" }.join(',') %>
    );

    tuple_transform(xs, [](auto x) {
        return x;
    });
}


#include "../<%=technique%>.hpp"


template <int i>
struct x { };

int main() {
    auto xs = make_tuple(
        <%= (1..n_elements).to_a.map{ |i| "x<#{i}>{}" }.join(',') %>
    );
    auto f = [](auto ...x) { };
    apply(f, xs);
}

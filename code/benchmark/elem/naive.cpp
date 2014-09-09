template <typename T, typename ...Ts>
struct elem {
    static constexpr bool value = false;
};

template <typename T, typename ...Us>
struct elem<T, T, Us...> {
    static constexpr bool value = true;
};

template <typename T, typename U, typename ...Us>
struct elem<T, U, Us...> {
    static constexpr bool value = elem<T, Us...>::value;
};

template <int> struct x;
<% xs = (1..n+1).map { |i| "x<#{i}>" }.join(', ') %>


int main() {
    bool r = elem<x<0>, <%= xs %>>::value;
}

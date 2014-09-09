template <typename T, typename U>
struct is_same { static constexpr bool value = false; };

template <typename T>
struct is_same<T, T> { static constexpr bool value = true; };


template <bool ...b>
struct any {
    static constexpr bool value = !is_same<
        any,
        any<(b, false)...>
    >::value;
};

template <typename key, typename ...keys>
constexpr bool elem = any<
    is_same<key, keys>::value...
>::value;


template <int> struct x;
<% xs = (1..n+1).map { |i| "x<#{i}>" }.join(', ') %>

constexpr bool r = elem<x<0>, <%= xs %>>;

int main() { }

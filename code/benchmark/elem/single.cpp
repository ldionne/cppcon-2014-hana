
struct yes { };
struct no { };

template <typename x>
struct no_decay { using type = x; };

template <typename ...keys>
struct map {
    template <typename fail = void>
    static no has_key(...);
};

template <typename key, typename ...keys>
struct map<key, keys...> : map<keys...> {
    using map<keys...>::has_key;
    static yes has_key(no_decay<key>*);
};

template <typename T, typename ...Ts>
using elem = decltype(
    map<Ts...>::has_key((no_decay<T>*)nullptr)
);


template <int> struct x;
<% xs = (1..n+1).map { |i| "x<#{i}>" }.join(', ') %>

using r = elem<x<0>, <%= xs %>>;

int main() { }

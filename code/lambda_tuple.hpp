// Copyright Louis Dionne 2014
// Distributed under the Boost Software License, Version 1.0.

#ifndef LAMBDA_TUPLE_HPP
#define LAMBDA_TUPLE_HPP

#include <cstddef>
#include <type_traits>
#include <utility>


//////////////////////////////////////////////////////////////////////////////
// tuple
//
// Note:
// The lambda in `make_storage` should actually capture like
// ```
//  t(std::forward<T>(t))...
// ```
// but this fails on both Clang and GCC.
//////////////////////////////////////////////////////////////////////////////
template <typename ...T>
/* constexpr */ decltype(auto) make_storage(T&& ...t) {
    return [t...](auto&& f) -> decltype(auto) {
        return std::forward<decltype(f)>(f)(t...);
    };
}

template <typename ...T>
struct tuple {
    using Storage = decltype(make_storage(std::declval<T>()...));
    Storage unpack_into;

    template <typename ...U>
    /* constexpr */ explicit tuple(U&& ...u)
        : unpack_into(make_storage(std::forward<U>(u)...))
    { };
};

template <typename ...T>
/* constexpr */ tuple<std::decay_t<T>...> make_tuple(T&& ...t) {
    return tuple<std::decay_t<T>...>{std::forward<T>(t)...};
}

//////////////////////////////////////////////////////////////////////////////
// get
//////////////////////////////////////////////////////////////////////////////
struct eat { template <typename ...X> constexpr eat(X&& ...) { } };

template <std::size_t n, typename = std::make_index_sequence<n>>
struct get_impl;

template <std::size_t n, std::size_t ...ignore>
struct get_impl<n, std::index_sequence<ignore...>> {
    template <typename Nth, typename ...Rest>
    constexpr decltype(auto) operator()
    (decltype(ignore, eat{})..., Nth&& nth, Rest&& ...) const
    { return std::forward<Nth>(nth); }
};

template <std::size_t n, typename Tuple>
/* constexpr */ decltype(auto) get(Tuple&& ts) {
    return std::forward<Tuple>(ts).unpack_into(get_impl<n>{});
}

//////////////////////////////////////////////////////////////////////////////
// transform
//////////////////////////////////////////////////////////////////////////////
template <typename Tuple, typename F>
/* constexpr */ decltype(auto) transform(Tuple&& ts, F&& f) {
    return std::forward<Tuple>(ts).unpack_into(
        [f(std::forward<F>(f))](auto&& ...ts) -> decltype(auto) {
            return make_tuple(f(std::forward<decltype(ts)>(ts))...);
        }
    );
}

//////////////////////////////////////////////////////////////////////////////
// the proposed C++17 "apply" function
//////////////////////////////////////////////////////////////////////////////
template <typename F, typename Tuple>
/* constexpr */ decltype(auto) apply(F&& f, Tuple&& ts) {
    return std::forward<Tuple>(ts).unpack_into(std::forward<F>(f));
}

#endif

// Copyright Louis Dionne 2014
// Distributed under the Boost Software License, Version 1.0.

#ifndef LAMBDA_TUPLE_HPP
#define LAMBDA_TUPLE_HPP

#include <cstddef>
#include <type_traits>
#include <utility>


namespace lambda {
//////////////////////////////////////////////////////////////////////////////
// std::tuple and std::make_tuple
//////////////////////////////////////////////////////////////////////////////
template <typename ...T>
/* constexpr */ decltype(auto) make_storage(T ...t) {
    // Should move-capture with `t{std::move(t)}...`, but this
    // fails on both Clang and GCC.
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
// std::get
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
// std::tuple_cat
//////////////////////////////////////////////////////////////////////////////
template <typename T1, typename T2, typename ...Ts>
/* constexpr */ decltype(auto) tuple_cat(T1&& t1, T2&& t2, Ts&& ...ts) {
    return tuple_cat(
        tuple_cat(std::forward<T1>(t1), std::forward<T2>(t2)),
        std::forward<Ts>(ts)...
    );
}

template <typename Tuple>
/* constexpr */ decltype(auto) tuple_cat(Tuple&& ts) {
    return std::forward<Tuple>(ts);
}

/* constexpr */ decltype(auto) tuple_cat() {
    return make_tuple();
}

template <typename Xs, typename Ys>
/* constexpr */ decltype(auto) tuple_cat(Xs&& xs, Ys&& ys) {
    return std::forward<Xs>(xs).unpack_into([&ys](auto&& ...x) -> decltype(auto) {
        return std::forward<Ys>(ys).unpack_into([&](auto&& ...y) -> decltype(auto) {
            // Should allow perfect forwarding here, but it's unclear how that
            // could be implemented.
            return make_tuple(
                x...,
                std::forward<decltype(y)>(y)...
            );
        });
    });
}

//////////////////////////////////////////////////////////////////////////////
// the proposed C++17 std::apply function
//////////////////////////////////////////////////////////////////////////////
template <typename F, typename Tuple>
/* constexpr */ decltype(auto) apply(F&& f, Tuple&& ts) {
    return std::forward<Tuple>(ts).unpack_into(std::forward<F>(f));
}

//////////////////////////////////////////////////////////////////////////////
// hypothetical std::tuple_transform
//////////////////////////////////////////////////////////////////////////////
template <typename Tuple, typename F>
/* constexpr */ decltype(auto) tuple_transform(Tuple&& ts, F&& f) {
    return std::forward<Tuple>(ts).unpack_into(
        [f(std::forward<F>(f))](auto&& ...ts) -> decltype(auto) {
            return make_tuple(f(std::forward<decltype(ts)>(ts))...);
        }
    );
}

//////////////////////////////////////////////////////////////////////////////
// hypothetical `std::tuple_for_each`
//////////////////////////////////////////////////////////////////////////////
template <typename Tuple, typename F>
/* constexpr */ void tuple_for_each(Tuple&& ts, F&& f) {
    std::forward<Tuple>(ts).unpack_into(
        [&](auto&& ...ts) {
            using swallow = int[];
            (void)swallow{1,
                (f(std::forward<decltype(ts)>(ts)), void(), 1)...
            };
        }
    );
}

//////////////////////////////////////////////////////////////////////////////
// front
//////////////////////////////////////////////////////////////////////////////
template <typename Tuple>
/* constexpr */ decltype(auto) front(Tuple&& ts) {
    auto fst = [](auto&& x, auto&& ...xs) -> decltype(auto) {
        return std::forward<decltype(x)>(x);
    };
    return std::forward<Tuple>(ts).unpack_into(fst);
}
} // end namespace lambda

#endif

// Copyright Louis Dionne 2014
// Distributed under the Boost Software License, Version 1.0.

#ifndef STD_TUPLE_HPP
#define STD_TUPLE_HPP

#include <cstddef>
#include <tuple>
#include <utility>


namespace standard {
//////////////////////////////////////////////////////////////////////////////
// std::tuple and std::make_tuple
//////////////////////////////////////////////////////////////////////////////
using std::tuple;
using std::make_tuple;

//////////////////////////////////////////////////////////////////////////////
// std::get
//////////////////////////////////////////////////////////////////////////////
using std::get;

//////////////////////////////////////////////////////////////////////////////
// std::tuple_cat
//////////////////////////////////////////////////////////////////////////////
using std::tuple_cat;

//////////////////////////////////////////////////////////////////////////////
// hypothetical std::tuple_transform
//////////////////////////////////////////////////////////////////////////////
template <typename Tuple, typename F, std::size_t ...i>
constexpr decltype(auto) tuple_transform_impl(Tuple&& ts, F f, std::index_sequence<i...>) {
    return make_tuple(f(get<i>(std::forward<Tuple>(ts)))...);
}

template <typename Tuple, typename F>
constexpr decltype(auto) tuple_transform(Tuple&& ts, F&& f) {
    return tuple_transform_impl(
        std::forward<Tuple>(ts),
        std::forward<F>(f),
        std::make_index_sequence<
            std::tuple_size<std::decay_t<Tuple>>::value
        >{}
    );
}

//////////////////////////////////////////////////////////////////////////////
// hypothetical `std::tuple_for_each`
//////////////////////////////////////////////////////////////////////////////
template <typename Tuple, typename F, std::size_t ...i>
constexpr void tuple_for_each_impl(Tuple&& ts, F f, std::index_sequence<i...>) {
    using swallow = int[];
    (void)swallow{1,
        (f(get<i>(std::forward<Tuple>(ts))), void(), 1)...
    };
}

template <typename Tuple, typename F>
constexpr void tuple_for_each(Tuple&& ts, F&& f) {
    tuple_for_each_impl(
        std::forward<Tuple>(ts),
        std::forward<F>(f),
        std::make_index_sequence<
            std::tuple_size<std::decay_t<Tuple>>::value
        >{}
    );
}

//////////////////////////////////////////////////////////////////////////////
// the proposed C++17 std::apply function
//////////////////////////////////////////////////////////////////////////////
template <typename F, typename Tuple, std::size_t ...i>
constexpr decltype(auto) apply_impl(F&& f, Tuple&& ts, std::index_sequence<i...>) {
    return std::forward<F>(f)(get<i>(std::forward<Tuple>(ts))...);
}

template <typename F, typename Tuple>
constexpr decltype(auto) apply(F&& f, Tuple&& ts) {
    return apply_impl(
        std::forward<F>(f),
        std::forward<Tuple>(ts),
        std::make_index_sequence<
            std::tuple_size<std::decay_t<Tuple>>::value
        >{}
    );
}
} // end namespace standard

#endif

// Copyright Louis Dionne 2014
// Distributed under the Boost Software License, Version 1.0.

#ifndef FUSION_VECTOR_HPP
#define FUSION_VECTOR_HPP

#define FUSION_MAX_VECTOR_SIZE 50
#include <boost/fusion/include/at_c.hpp>
#include <boost/fusion/include/for_each.hpp>
#include <boost/fusion/include/join.hpp>
#include <boost/fusion/include/make_fused.hpp>
#include <boost/fusion/include/transform.hpp>
#include <boost/fusion/include/vector.hpp>

#include <type_traits>
#include <utility>


//////////////////////////////////////////////////////////////////////////////
// std::tuple and std::make_tuple
//////////////////////////////////////////////////////////////////////////////
template <typename ...T>
using tuple = boost::fusion::vector<T...>;

template <typename ...T>
constexpr tuple<std::decay_t<T>...> make_tuple(T&& ...t) {
    return tuple<std::decay_t<T>...>{std::forward<T>(t)...};
}

//////////////////////////////////////////////////////////////////////////////
// std::get
//////////////////////////////////////////////////////////////////////////////
template <std::size_t n, typename Tuple>
constexpr decltype(auto) get(Tuple&& ts) {
    return boost::fusion::at_c<n>(std::forward<Tuple>(ts));
}

//////////////////////////////////////////////////////////////////////////////
// std::tuple_cat
//////////////////////////////////////////////////////////////////////////////
template <typename T1, typename T2, typename ...Ts>
constexpr decltype(auto) tuple_cat(T1&& t1, T2&& t2, Ts&& ...ts) {
    return tuple_cat(
        tuple_cat(std::forward<T1>(t1), std::forward<T2>(t2)),
        std::forward<Ts>(ts)...
    );
}

template <typename Tuple>
constexpr decltype(auto) tuple_cat(Tuple&& ts) {
    return std::forward<Tuple>(ts);
}

decltype(auto) tuple_cat() {
    return make_tuple();
}

template <typename Xs, typename Ys>
constexpr decltype(auto) tuple_cat(Xs&& xs, Ys&& ys) {
    return boost::fusion::join(std::forward<Xs>(xs), std::forward<Ys>(ys));
}

//////////////////////////////////////////////////////////////////////////////
// the proposed C++17 std::apply function
//////////////////////////////////////////////////////////////////////////////
template <typename F, typename Tuple>
constexpr decltype(auto) apply(F&& f, Tuple&& ts) {
    return boost::fusion::make_fused(std::forward<F>(f))(std::forward<Tuple>(ts));
}

//////////////////////////////////////////////////////////////////////////////
// hypothetical std::tuple_transform
//////////////////////////////////////////////////////////////////////////////
template <typename Tuple, typename F>
constexpr decltype(auto) tuple_transform(Tuple&& ts, F&& f) {
    return boost::fusion::transform(std::forward<Tuple>(ts), std::forward<F>(f));
}

//////////////////////////////////////////////////////////////////////////////
// hypothetical `std::tuple_for_each`
//////////////////////////////////////////////////////////////////////////////
template <typename Tuple, typename F>
constexpr void tuple_for_each(Tuple&& ts, F&& f) {
    boost::fusion::for_each(std::forward<Tuple>(ts), std::forward<F>(f));
}

#endif

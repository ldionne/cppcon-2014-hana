// Copyright Louis Dionne 2014
// Distributed under the Boost Software License, Version 1.0.

#include <boost/hana/bool.hpp>
#include <boost/hana/core/datatype.hpp>
#include <boost/hana/core/is_a.hpp>
#include <boost/hana/foldable/folds_mcd.hpp>
#include <boost/hana/functional/fix.hpp>
#include <boost/hana/functional/id.hpp>
#include <boost/hana/functional/placeholder.hpp>
#include <boost/hana/functor/fmap_mcd.hpp>
#include <boost/hana/tuple.hpp>

#include <type_traits>
#include <utility>


//////////////////////////////////////////////////////////////////////////////
// Expression
//////////////////////////////////////////////////////////////////////////////
struct Expression { };

template <typename Op, typename Args>
struct function_node {
    Op op;
    Args args;
    using hana_datatype = Expression;
    static constexpr bool is_function = true;
};

template <typename T>
struct terminal_node {
    T value;
    using hana_datatype = Expression;
    static constexpr bool is_terminal = true;
};

template <typename T>
using strip_t = std::remove_cv_t<std::remove_reference_t<T>>;

template <typename T, typename = void>
constexpr auto is_terminal = boost::hana::false_;

template <typename T>
constexpr auto is_terminal<T, decltype((void)strip_t<T>::is_terminal)> = boost::hana::true_;

template <typename F, typename = void>
constexpr auto is_function = boost::hana::false_;

template <typename F>
constexpr auto is_function<F, decltype((void)strip_t<F>::is_function)> = boost::hana::true_;

template <typename T>
constexpr auto is_Expression = boost::hana::bool_<
    std::is_same<Expression, boost::hana::datatype_t<T>>::value
>;


auto terminal = [](auto&& x) -> decltype(auto) {
    auto make = [](auto&& y) -> decltype(auto) {
        return terminal_node<std::decay_t<decltype(y)>>{
            std::forward<decltype(y)>(y)
        };
    };
    return boost::hana::if_(is_Expression<decltype(x)>,
        boost::hana::id,
        make
    )(std::forward<decltype(x)>(x));
};

// `f` is a function object called on the evaluated `args...` when the
// expression itself is evaluated.
//
// `args...` are 0 or more objects representing the arguments to `f`. `args...`
// are passed through `terminal`; so `Expression`s can also be used, in which
// case they are left as-is.
//
// note:
// `function(f)` may only be called a single time, because we're
// moving `f` out.
auto function = [](auto&& f) -> decltype(auto) {
    return [f(std::forward<decltype(f)>(f))](auto&& ...args) -> decltype(auto) {
        auto a = boost::hana::tuple(terminal(std::forward<decltype(args)>(args))...);
        return function_node<std::decay_t<decltype(f)>, decltype(a)>{
            std::move(f), std::move(a)
        };
    };
};

template <typename T, typename = std::enable_if_t<is_terminal<T>()>>
constexpr decltype(auto) eval_impl(T&& t)
{ return std::forward<T>(t).value; }

auto eval = [](auto&& expr) -> decltype(auto) {
    return eval_impl(std::forward<decltype(expr)>(expr));
};

template <typename F, typename = void, typename = std::enable_if_t<is_function<F>()>>
constexpr decltype(auto) eval_impl(F&& f) {
    return boost::hana::unpack(
        boost::hana::fmap(std::forward<F>(f).args, eval),
        std::forward<F>(f).op
    );
}


namespace boost { namespace hana {
    template <>
    struct Functor::instance<Expression> : Functor::fmap_mcd {
        template <typename Op, typename Args, typename F>
        static constexpr decltype(auto) fmap_impl(function_node<Op, Args> e, F f) {
            auto g = [=](auto&& arg) -> decltype(auto) {
                return fmap(std::forward<decltype(arg)>(arg), f);
            };
            return unpack(fmap(e.args, g), function(e.op));
        }

        template <typename T, typename F>
        static constexpr decltype(auto) fmap_impl(terminal_node<T> e, F&& f) {
            return terminal(std::forward<F>(f)(e.value));
        }
    };

    template <>
    struct Foldable::instance<Expression> : Foldable::folds_mcd {
        template <typename Op, typename Args, typename S, typename F>
        static constexpr decltype(auto) foldl_impl(function_node<Op, Args> e, S&& s, F f) {
            return foldl(e.args, std::forward<S>(s),
                [=](auto&& state, auto&& arg) -> decltype(auto) {
                    return foldl(
                        std::forward<decltype(arg)>(arg),
                        std::forward<decltype(state)>(state),
                        f
                    );
                });
        }

        template <typename T, typename S, typename F>
        static constexpr decltype(auto) foldl_impl(terminal_node<T> e, S&& s, F&& f) {
            return std::forward<F>(f)(std::forward<S>(s), e.value);
        }


        template <typename Op, typename Args, typename S, typename F>
        static constexpr decltype(auto) foldr_impl(function_node<Op, Args> e, S&& s, F f) {
            return foldr(e.args, std::forward<S>(s),
                [=](auto&& arg, auto&& state) -> decltype(auto) {
                    return foldr(
                        std::forward<decltype(arg)>(arg),
                        std::forward<decltype(state)>(state),
                        f
                    );
                });
        }

        template <typename T, typename S, typename F>
        static constexpr decltype(auto) foldr_impl(terminal_node<T> e, S&& s, F&& f) {
            return std::forward<F>(f)(e.value, std::forward<S>(s));
        }
    };
}}

#define BINARY_NODE(OP)                                                     \
    template <typename E1, typename E2, typename = std::enable_if_t<        \
        is_Expression<E1>() || is_Expression<E2>()                          \
    >>                                                                      \
    decltype(auto) operator OP(E1&& e1, E2&& e2) {                          \
        auto f = boost::hana::_ OP boost::hana::_;                          \
        return function(f)(std::forward<E1>(e1), std::forward<E2>(e2));     \
    }                                                                       \
/**/

BINARY_NODE(+)
BINARY_NODE(-)
BINARY_NODE(*)
BINARY_NODE(/)


//////////////////////////////////////////////////////////////////////////////
// Tests
//////////////////////////////////////////////////////////////////////////////

#include <boost/hana/detail/assert.hpp>
#include <boost/hana/integral.hpp>

#include <sstream>
#include <string>
using namespace boost::hana;


int main() {
    // eval
    BOOST_HANA_RUNTIME_ASSERT(eval(function(_ + _)(1, 2)) == 1 + 2);
    BOOST_HANA_RUNTIME_ASSERT(eval(terminal(1) + 2) == 1 + 2);
    BOOST_HANA_RUNTIME_ASSERT(eval(terminal(1) + 2 + 3) == 1 + 2 + 3);
    BOOST_HANA_RUNTIME_ASSERT(eval(terminal(1) + 2 + 3 - 4) == 1 + 2 + 3 - 4);
    BOOST_HANA_CONSTANT_ASSERT(
        eval(terminal(int_<1>) + terminal(int_<2>) + terminal(int_<3>))
        ==
        int_<1 + 2 + 3>
    );

    // Functor
    auto to_s = [](auto x) { return (std::ostringstream{} << x).str(); };
    auto inc = [](auto x) { return x + 1; };
    BOOST_HANA_RUNTIME_ASSERT(eval(fmap(terminal(1) + 2 + 3, inc)) == 2 + 3 + 4);
    BOOST_HANA_RUNTIME_ASSERT(eval(fmap(terminal(1) + 2 + 3, to_s)) == "123");


    // Foldable
    BOOST_HANA_RUNTIME_ASSERT(
        foldl(terminal("1") + "2" + "3", std::string{}, _+_)
        ==
        "123"
    );

    BOOST_HANA_RUNTIME_ASSERT(
        foldr(terminal("1") + "2" + "3", std::string{}, _+_)
        ==
        "123"
    );
}

// Copyright Louis Dionne 2014
// Distributed under the Boost Software License, Version 1.0.

#include <boost/hana/detail/assert.hpp>
#include <boost/hana/functional/placeholder.hpp>
#include <boost/hana/integral.hpp>
#include <boost/hana/tuple.hpp>

#include <type_traits>
#include <utility>
#include <vector>
using namespace boost::hana;
using namespace literals;


//////////////////////////////////////////////////////////////////////////////
// Tree creation
//////////////////////////////////////////////////////////////////////////////
template <typename T> struct treeify : std::false_type { };
template <typename T> struct treeify<T const> : treeify<T> { };
template <typename T> struct treeify<T&> : treeify<T> { };
template <typename T> struct treeify<T&&> : treeify<T> { };


template <typename Derived>
struct expression_base;

template <typename X>
struct terminal_type : expression_base<terminal_type<X>> {
    X value;

    explicit constexpr terminal_type(X x)
        : value(x)
    { }
};

template <typename F, typename ...Args>
struct function_type : expression_base<function_type<F, Args...>> {
    F value;
    using Storage = decltype(tuple(std::declval<Args>()...));
    Storage args;

    explicit constexpr function_type(F f, Args ...a)
        : value(f), args(tuple(a...))
    { }
};

auto terminal = [](auto x) {
    return terminal_type<decltype(x)>(x);
};

auto function = [](auto f, auto ...args) {
    return function_type<decltype(f), decltype(args)...>(f, args...);
};

struct subscript_tag {
    template <typename T, typename I>
    constexpr decltype(auto) operator()(T&& t, I&& i) const
    { return std::forward<T>(t)[std::forward<I>(i)]; }
};

template <typename Derived>
struct expression_base {
    template <typename T>
    constexpr decltype(auto) operator[](T&& t) const {
        return function(subscript_tag{},
            static_cast<Derived const&>(*this),
            std::forward<T>(t)
        );
    }
};


template <typename X>
struct treeify<terminal_type<X>> : std::true_type { };

template <typename F, typename ...Args>
struct treeify<function_type<F, Args...>> : std::true_type { };

#define TREEIFY_BINARY_OP(OP, TAG)                                          \
    struct TAG {                                                            \
        template <typename T, typename U>                                   \
        constexpr decltype(auto) operator()(T&& t, U&& u) const             \
        { return std::forward<T>(t) OP std::forward<U>(u); }                \
    };                                                                      \
                                                                            \
    template <typename T, typename U, typename = std::enable_if_t<          \
        treeify<T>::value && treeify<U>::value                              \
    >>                                                                      \
    constexpr decltype(auto) operator OP (T&& t, U&& u) {                   \
        return function(TAG{}, std::forward<T>(t), std::forward<U>(u));     \
    }                                                                       \
    static_assert(true, "this is used just to allow a trailing semicolon")  \
/**/

TREEIFY_BINARY_OP(+, plus_tag);
TREEIFY_BINARY_OP(-, minus_tag);
TREEIFY_BINARY_OP(*, times_tag);
TREEIFY_BINARY_OP(/, divide_tag);

template <typename Derived>
struct evaluator {
    template <typename F, typename ...Args>
    constexpr decltype(auto) operator()(function_type<F, Args...> f) const {
        return boost::hana::unpack(
            boost::hana::fmap(f.args, static_cast<Derived const&>(*this)),
            f.value
        );
    }

    template <typename X>
    constexpr decltype(auto) operator()(terminal_type<X> x) const {
        return x.value;
    }
};

constexpr struct eval_type : evaluator<eval_type> { } eval{};


//////////////////////////////////////////////////////////////////////////////
// Matrix
//////////////////////////////////////////////////////////////////////////////
struct Matrix {
    // storage[i][j] means the (i,j)th element in the matrix.
    using Row = std::vector<int>;
    std::vector<Row> storage;

    decltype(auto) operator[](std::pair<int, int> index) const
    { return storage[index.first][index.second]; }
};

constexpr struct matrix_evaluator : evaluator<matrix_evaluator> {
    template <typename Index, typename M1, typename M2>
    constexpr decltype(auto) operator()(
        function_type<subscript_tag,
            function_type<plus_tag, M1, M2>,
            terminal_type<Index>
        > expr
    ) const {
        auto sum = expr.args[0_c];
        auto index = expr.args[1_c];
        return (*this)(sum.args[0_c][index]) +
               (*this)(sum.args[1_c][index]);
    }

    using evaluator<matrix_evaluator>::operator();
} matrix_eval{};


//////////////////////////////////////////////////////////////////////////////
// Tests
//////////////////////////////////////////////////////////////////////////////

int main() {
    // eval
    {
        auto t = terminal(1) + terminal(3);
        BOOST_HANA_RUNTIME_ASSERT(eval(t) == 1 + 3);

        BOOST_HANA_RUNTIME_ASSERT(eval(
            function(_+_, terminal(4), terminal(5))
        ) == 4 + 5);
    }

    // matrix_eval
    {
        Matrix m1{{{1, 2}, {3, 4}}},
               m2{{{5, 6}, {7, 8}}},
               m3{{{9, 10}, {11, 12}}};

        BOOST_HANA_RUNTIME_ASSERT(matrix_eval(
            terminal(1) + terminal(2)
        ) == 1 + 2);

        BOOST_HANA_RUNTIME_ASSERT(matrix_eval(
            (terminal(m1) + terminal(m2))[terminal(std::make_pair(1, 0))]
        ) == 3 + 7);

        BOOST_HANA_RUNTIME_ASSERT(matrix_eval(
            (terminal(m1) + terminal(m2) + terminal(m3))[
                terminal(std::make_pair(1, 0))
            ]
        ) == 3 + 7 + 11);
    }
}

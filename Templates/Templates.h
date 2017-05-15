#pragma once

#include <tuple>

#include "TypeList.h"

namespace detail {
    template<size_t M, size_t N>
    struct IndexPair {
        static constexpr size_t first = M;
        static constexpr size_t second = N;
    };

    struct Empty {};

    template<bool expr, typename a, typename b>
    struct TemplateBooleanIfElse {};

    template<typename a, typename b>
    struct TemplateBooleanIfElse<true, a, b> {
        using type = a;
    };

    template<typename a, typename b>
    struct TemplateBooleanIfElse<false, a, b> {
        using type = b;
    };

    template<size_t nI, size_t First, size_t Second, size_t ...Is>
    struct NextIntPair {
        using PairType = IndexPair<First, Second>;
        using NextType = NextIntPair<(sizeof ...(Is)) - 1, Second, Is...>;
    };

    template<size_t First, size_t Second>
    struct NextIntPair<0, First, Second> {
        using PairType = IndexPair<First, Second>;
        using NextType = Empty;
    };

    template<typename, typename>
    struct AppendTypeSequence {};

    template<typename T, typename ...Ts>
    struct AppendTypeSequence<T, std::tuple<Ts...>> {
        using type = std::tuple<Ts..., T>;
    };

    template<typename ...Ts>
    struct ReverseTypeSequence {
        using type = std::tuple<>;
    };

    template<typename T, typename ...Ts>
    struct ReverseTypeSequence<T, Ts...> {
        using type = typename AppendTypeSequence<
            T, typename ReverseTypeSequence<Ts...>::type
        >::type;
    };
}

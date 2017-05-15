#pragma once

#include <tuple>

namespace detail {
    template<typename ...Ts>
    struct TypeList;

    template<typename FirstType, typename ...Ts>
    struct TypeLinkedList {
        using Head = FirstType;
        using Tail = TypeLinkedList<Ts...>;
    };

    template<size_t N, typename TypeLinkedList, typename ...Ts>
    struct TypeListPopBackImpl
        : TypeListPopBackImpl<
            N - 1,
            typename TypeLinkedList::Tail,
            Ts...,
            typename TypeLinkedList::Head> {};

    template<typename TypeLinkedList, typename ...Ts>
    struct TypeListPopBackImpl<0, TypeLinkedList, Ts...> {
        using type = TypeList<Ts...>;
    };

    template<typename ...Ts>
    struct TypeListPopBack
        : TypeListPopBackImpl<sizeof ...(Ts) - 1, TypeLinkedList<Ts...>> {};

    template<typename ...Ts>
    struct TypeList {
        using TupleType = std::tuple<Ts...>;
        using NextType = typename TypeListPopBack<Ts...>::type;
    };

    template<typename TupleType, size_t N, size_t ...Is>
    struct TupleTypeListGeneratorImpl
        : TupleTypeListGeneratorImpl<TupleType, N - 1, N - 1, N, Is...> {};

    template<typename TupleType, size_t ...Is>
    struct TupleTypeListGeneratorImpl<TupleType, 0, Is...> {
        using type = TypeList<typename std::tuple_element<Is, TupleType>::type...>;
    };

    template<typename TupleType>
    struct TupleTypeListGenerator
        : TupleTypeListGeneratorImpl<
            TupleType, std::tuple_size<TupleType>::value - 1> {};
}

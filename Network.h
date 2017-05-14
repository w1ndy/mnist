#pragma once

#include "Vector.h"
#include "Matrix.h"

#include <tuple>
#include <random>
#include <functional>
#include <iostream>
#include <utility>

namespace detail {
    template<typename T, size_t ...Is>
    void invokePrintImpl(T const &tuple, std::index_sequence<Is...>) {
        int _[] = {(std::get<Is>(tuple).print(), 0)...};
    }

    template<typename T>
    void invokePrint(T const &tuple) {
        invokePrintImpl(
            tuple, std::make_index_sequence<std::tuple_size<T>::value>());
    }

    template<int First, int ...Remains>
    class BiasesImpl
    {
    public:
        std::tuple<Vector<Remains> ...> value{
            Vector<Remains>([](int) {
                static std::random_device rd;
                static std::mt19937 gen(rd());
                static std::normal_distribution<> dist(0, 1);
                return dist(gen);
            })...
        };
    };

    template<size_t M, size_t N>
    struct IndexPair {
        static constexpr size_t first = M;
        static constexpr size_t second = N;
    };

    struct Empty {};

    template<int nI, int First, int Second, int ...Is>
    struct NextIntPair {
        using PairType = IndexPair<First, Second>;
        using NextType = NextIntPair<(sizeof ...(Is)) - 1, Second, Is...>;
    };

    template<int First, int Second>
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

    template<typename Is, typename ...Ts>
    struct WeightsTypeGenerator
        : WeightsTypeGenerator<
            typename Is::NextType,
            typename Is::PairType,
            Ts...> {};

    template<typename ...Ts>
    struct WeightsTypeGenerator<Empty, Ts...> {
        using type = typename ReverseTypeSequence<
            Matrix<Ts::second, Ts::first>...
        >::type;
    };

    template<int ...LayerDesc>
    class WeightsImpl
    {
    public:
        using ValueType = typename WeightsTypeGenerator<
            NextIntPair<sizeof ...(LayerDesc) - 2, LayerDesc...>
        >::type;

        ValueType value;

    public:
        template<int First, int ...Rest>
        auto _initializeValue() {
            return ValueType((sizeof(Rest), [](int, int) {
                static std::random_device rd;
                static std::mt19937 gen(rd());
                static std::normal_distribution<> dist(0, 1);
                return dist(gen);
            })...);
        }

        WeightsImpl()
            : value(_initializeValue<LayerDesc...>()) {};
    };
}

template<int ... LayerDesc>
class Network
{
private:
    detail::BiasesImpl<LayerDesc ...> _biases;
    detail::WeightsImpl<LayerDesc ...> _weights;

public:
    Network() {
        detail::invokePrint(_biases.value);
        detail::invokePrint(_weights.value);
    }
};

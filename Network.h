#pragma once

#include "Templates.h"
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

    template<size_t First, size_t ...Remains>
    class BiasesImpl
    {
    public:
        std::tuple<Vector<Remains> ...> value{
            Vector<Remains>([](size_t) {
                static std::random_device rd;
                static std::mt19937 gen(rd());
                static std::normal_distribution<> dist(0, 1);
                return dist(gen);
            })...
        };
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

    template<size_t ...LayerDesc>
    class WeightsImpl
    {
    public:
        using ValueType = typename WeightsTypeGenerator<
            NextIntPair<sizeof ...(LayerDesc) - 2, LayerDesc...>
        >::type;

        ValueType value;

    public:
        template<size_t First, size_t ...Rest>
        auto _initializeValue() {
            return ValueType((sizeof(Rest), [](size_t, size_t) {
                static std::random_device rd;
                static std::mt19937 gen(rd());
                static std::normal_distribution<> dist(0, 1);
                return dist(gen);
            })...);
        }

        WeightsImpl()
            : value(_initializeValue<LayerDesc...>()) {};
    };

    template<size_t First, size_t ...Rest>
    struct NetworkInputSize {
        static size_t constexpr value = First;
    };

    template<size_t RestSize, size_t First, size_t ...Rest>
    struct NetworkOutputSizeImpl
        : NetworkOutputSizeImpl<RestSize - 1, Rest...> {};

    template<size_t First, size_t ...Rest>
    struct NetworkOutputSizeImpl<0, First, Rest...> {
        static size_t constexpr value = First;
    };

    template<size_t ...LayerDesc>
    struct NetworkOutputSize {
        static size_t constexpr value =
            NetworkOutputSizeImpl<sizeof ...(LayerDesc) - 1, LayerDesc...>::value;
    };

    template<size_t ...LayerDesc>
    struct NetworkLayerSize {
        static size_t constexpr value = sizeof ...(LayerDesc);
    };

    template<
        typename WeightsType, typename BiasesType,
        typename InputType, typename OutputTypeList,
        size_t nI, size_t ...Is>
    typename OutputTypeList::TupleType runNetworkImpl(
        WeightsType const &ws, BiasesType const &bs,
        InputType const &in, std::index_sequence<Is...>, int) {
            return typename OutputTypeList::TupleType(
                std::get<0>(ws) * in + std::get<0>(bs));
        }

    template<
        typename WeightsType, typename BiasesType,
        typename InputType, typename OutputTypeList,
        size_t nI, size_t ...Is>
    typename OutputTypeList::TupleType runNetworkImpl(
        WeightsType const &ws, BiasesType const &bs,
        InputType const &in, std::index_sequence<Is...>,
        void *tag = 0) {
            typename OutputTypeList::NextType::TupleType prev = runNetworkImpl<
                WeightsType, BiasesType,
                InputType, typename OutputTypeList::NextType, nI - 1>(
                    ws, bs, in, std::make_index_sequence<nI - 1>(),
                    typename TemplateBooleanIfElse<nI == 2, int, void*>::type(0)
                );
            return typename OutputTypeList::TupleType(
                std::get<Is>(prev)...,
                std::get<nI - 1>(ws) * std::get<nI - 2>(prev)
                    + std::get<nI - 1>(bs)
            );
        }

    template<
        typename WeightsType, typename BiasesType,
        typename InputType, typename OutputType,
        size_t ...LayerDesc>
    OutputType runNetwork(
        WeightsType const &ws, BiasesType const &bs, InputType const &in) {
            return runNetworkImpl<
                WeightsType, BiasesType,
                InputType, typename TupleTypeListGenerator<OutputType>::type,
                sizeof ...(LayerDesc) - 1>(
                    ws, bs, in,
                    std::make_index_sequence<sizeof ...(LayerDesc) - 2>()
                );
        }
}

template<size_t ...LayerDesc>
class Network
{
private:
    detail::BiasesImpl<LayerDesc...> _biases;
    detail::WeightsImpl<LayerDesc...> _weights;

public:
    using WeightsType = decltype(_weights.value);
    using BiasesType = decltype(_biases.value);

    using InputVectorType = Vector<detail::NetworkInputSize<LayerDesc...>::value>;
    using OutputVectorType = Vector<detail::NetworkOutputSize<LayerDesc...>::value>;
    using OutputSequenceType = BiasesType;
    using LayerSize = detail::NetworkLayerSize<LayerDesc...>;

public:
    Network() {
        detail::invokePrint(_biases.value);
        detail::invokePrint(_weights.value);

        std::cout << detail::NetworkInputSize<LayerDesc...>::value << std::endl;
        std::cout << detail::NetworkOutputSize<LayerDesc...>::value << std::endl;

        detail::invokePrint(runSequence({0.2, 0.3}));
    }

    OutputSequenceType runSequence(InputVectorType const &iv) {
        return detail::runNetwork<
            WeightsType, BiasesType,
            InputVectorType, OutputSequenceType, LayerDesc...>(
                _weights.value, _biases.value, iv
            );
    }
};

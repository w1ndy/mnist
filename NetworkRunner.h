#pragma once

#include <tuple>

namespace detail
{
    template<
        typename WeightsType, typename BiasesType,
        template<typename T> typename ActivatorType,
        typename InputType, typename OutputTypeList,
        size_t nI, size_t ...Is>
    typename OutputTypeList::TupleType runNetworkImpl(
        WeightsType const &ws, BiasesType const &bs,
        InputType const &in, std::index_sequence<Is...>, int)
    {
        return typename OutputTypeList::TupleType(
            ActivatorType<
                typename std::tuple_element<0, BiasesType>::type
            >::run(std::get<0>(ws) * in + std::get<0>(bs))
        );
    }

    template<
        typename WeightsType, typename BiasesType,
        template<typename T> typename ActivatorType,
        typename InputType, typename OutputTypeList,
        size_t nI, size_t ...Is>
    typename OutputTypeList::TupleType runNetworkImpl(
        WeightsType const &ws, BiasesType const &bs,
        InputType const &in, std::index_sequence<Is...>,
        void *tag = 0)
    {
        typename OutputTypeList::NextType::TupleType prev = runNetworkImpl<
            WeightsType, BiasesType, ActivatorType,
            InputType, typename OutputTypeList::NextType, nI - 1>(
                ws, bs, in, std::make_index_sequence<nI - 1>(),
                typename TemplateBooleanIfElse<nI == 2, int, void*>::type(0)
            );
        return typename OutputTypeList::TupleType(
            std::get<Is>(prev)...,
            ActivatorType<
                typename std::tuple_element<nI - 1, BiasesType>::type
            >::run(
                std::get<nI - 1>(ws) * std::get<nI - 2>(prev)
                    + std::get<nI - 1>(bs)
            )
        );
    }

    template<
        typename WeightsType, typename BiasesType,
        template<typename T> typename ActivatorType,
        typename InputType, typename OutputType,
        size_t ...LayerDesc>
    OutputType runNetwork(
        WeightsType const &ws, BiasesType const &bs, InputType const &in)
    {
        return runNetworkImpl<
            WeightsType, BiasesType, ActivatorType,
            InputType, typename TupleTypeListGenerator<OutputType>::type,
            sizeof ...(LayerDesc) - 1
        >(
            ws, bs, in,
            std::make_index_sequence<sizeof ...(LayerDesc) - 2>()
        );
    }
}
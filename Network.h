#pragma once

#include "Templates.h"
#include "Vector.h"
#include "Matrix.h"

#include "NetworkTypes.h"
#include "NetworkTraits.h"
#include "NetworkRunner.h"
#include "NetworkActivator.h"

#include <tuple>
#include <random>
#include <functional>
#include <iostream>
#include <utility>

namespace detail
{
    template<typename T, size_t ...Is>
    void invokePrintImpl(T const &tuple, std::index_sequence<Is...>)
    {
        int _[] = {(std::get<Is>(tuple).print(), 0)...};
    }

    template<typename T>
    void invokePrint(T const &tuple)
    {
        invokePrintImpl(
            tuple, std::make_index_sequence<std::tuple_size<T>::value>());
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
    Network()
    {
        detail::invokePrint(_biases.value);
        detail::invokePrint(_weights.value);

        std::cout << detail::NetworkInputSize<LayerDesc...>::value << std::endl;
        std::cout << detail::NetworkOutputSize<LayerDesc...>::value << std::endl;

        detail::invokePrint(runSequence({0.2, 0.3}));
    }

    OutputSequenceType runSequence(InputVectorType const &iv)
    {
        return detail::runNetwork<
            WeightsType, BiasesType, detail::SigmodActivator,
            InputVectorType, OutputSequenceType, LayerDesc...>(
                _weights.value, _biases.value, iv
            );
    }
};

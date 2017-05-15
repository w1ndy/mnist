#pragma once

#include <tuple>
#include <random>
#include <functional>

#include "../Math/Vector.h"
#include "../Math/Matrix.h"

#include "../Templates/Templates.h"

namespace detail
{
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
    struct WeightsTypeGenerator<Empty, Ts...>
    {
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
}
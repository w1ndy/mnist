#pragma once

namespace detail
{
    template<size_t First, size_t ...Rest>
    struct NetworkInputSize
    {
        static size_t constexpr value = First;
    };

    template<size_t RestSize, size_t First, size_t ...Rest>
    struct NetworkOutputSizeImpl
        : NetworkOutputSizeImpl<RestSize - 1, Rest...> {};

    template<size_t First, size_t ...Rest>
    struct NetworkOutputSizeImpl<0, First, Rest...>
    {
        static size_t constexpr value = First;
    };

    template<size_t ...LayerDesc>
    struct NetworkOutputSize
    {
        static size_t constexpr value =
            NetworkOutputSizeImpl<sizeof ...(LayerDesc) - 1, LayerDesc...>::value;
    };

    template<size_t ...LayerDesc>
    struct NetworkLayerSize
    {
        static size_t constexpr value = sizeof ...(LayerDesc);
    };
}
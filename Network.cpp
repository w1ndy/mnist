#include "Network.h"

template<int ... LayerDesc>
Vector Network::_biases[sizeof...(LayerDesc)] = {
    Vector<LayerDesc>()...
};

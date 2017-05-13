#pragma once

#include "Vector.h"

template<int ... LayerDesc>
class Network
{
private:
    Vector _biases[sizeof...(LayerDesc)];
};

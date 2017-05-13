#pragma once

#include "Vector.h"

#include <tuple>
#include <random>
#include <functional>
#include <utility>

template<int ... LayerDesc>
class Network
{
private:
    const std::function<double (int)> _generator = [](int) {
        static std::random_device rd;
        static std::mt19937 gen(rd());
        static std::normal_distribution<> dist(0, 1);
        return dist(gen);
    };

    std::tuple<Vector<LayerDesc> ...> _biases = {
        Vector<LayerDesc>(_generator) ...
    };

public:
    Network() {
        std::apply([](auto const &b) { b.print() }, _biases);
    }
};

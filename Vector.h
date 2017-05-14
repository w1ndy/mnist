#pragma once

#include <iostream>
#include <cstring>
#include <functional>
#include <iomanip>

namespace detail {
    template<size_t N, typename F, size_t ...Is>
    inline auto fillImpl(F const &generator, std::index_sequence<Is...>) {
        return std::array<double, N>{ generator(Is)... };
    }

    template<size_t N, typename F>
    inline auto fill(F const &generator) {
        return fillImpl<N>(generator, std::make_index_sequence<N>());
    }
}

template<int Size>
class Vector
{
private:
    std::array<double, Size> _data;

public:
    Vector() {};

    Vector(std::function<double (int)> const &generator)
        : _data(detail::fill<Size>(generator)) {};

    constexpr int size() {
        return Size;
    }

    void print() const {
        std::cout << std::setprecision(4) << '[' << std::endl;
        for (auto &d : _data) {
            std::cout << "  " << std::setw(10) << d << ',' << std::endl;
        }
        std::cout << ']' << std::endl;
    }
};

#pragma once

#include <tuple>

namespace detail {
    template<size_t M, size_t N, typename F, size_t ...Is>
    inline auto fill2DImpl(F const &generator, std::index_sequence<Is...>) {
        return std::array<double, M * N>{ generator(Is / N, Is % N)... };
    }

    template<size_t M, size_t N, typename F>
    inline auto fill2D(F const &generator) {
        return fill2DImpl<M, N>(generator, std::make_index_sequence<M * N>());
    }
}

template<int M, int N>
class Matrix
{
private:
    std::array<double, M * N> _data;

public:
    Matrix() {};

    Matrix(std::function<double (int, int)> const &generator)
        : _data(detail::fill2D<M, N>(generator)) {};

    void print() const {
        std::cout << std::setprecision(4) << '[' << std::endl;
        for (int i = 0; i < M * N; i += N) {
            std::cout << "  [";
            for (int j = 0; j < N; j++) {
                std::cout << std::setw(10) << _data[i + j] << ',';
            }
            std::cout << ']' << std::endl;
        }
        std::cout << ']' << std::endl;
    }
};

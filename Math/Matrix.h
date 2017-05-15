#pragma once

#include <tuple>
#include <cassert>

#include "Vector.h"

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

template<size_t M, size_t N>
class Matrix
{
private:
    std::array<double, M * N> _data;

public:
    Matrix() {};

    Matrix(std::function<double (size_t, size_t)> const &generator)
        : _data(detail::fill2D<M, N>(generator)) {};

    Matrix(std::initializer_list<double> const &l) : _data(l) {};

    Matrix(Matrix const &m) : _data(m._data) {};
    Matrix(Matrix const &&m) : _data(std::move(m._data)) {};

    void print() const {
        std::cout << std::setprecision(4) << '[' << std::endl;
        for (size_t i = 0; i < M * N; i += N) {
            std::cout << "  [";
            for (size_t j = 0; j < N; j++) {
                std::cout << std::setw(10) << _data[i + j] << ',';
            }
            std::cout << ']' << std::endl;
        }
        std::cout << ']' << std::endl;
    }

    double &operator ()(size_t i, size_t j) {
        assert(i < M && j < N && "matrix.get: argument out of bound!");
        return _data[i * N + j];
    }

    double const &operator ()(size_t i, size_t j) const {
        assert(i < M && j < N && "matrix.get: argument out of bound!");
        return _data[i * N + j];
    }

    Vector<M> operator*(Vector<N> const &v) const {
        Vector<M> rv;
        std::cout << "multiplying" << std::endl;
        this->print();
        v.print();
        for (int i = 0; i < M; i++) {
            for (int j = 0; j < N; j++) {
                rv._data[i] = _data[i * N + j] * v._data[j];
            }
        }
        std::cout << "yields" << std::endl;
        rv.print();
        std::cout << "---" << std::endl;
        return rv;
    }
};

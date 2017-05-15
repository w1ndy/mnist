#pragma once

#include <iostream>
#include <cstring>
#include <functional>
#include <iomanip>
#include <cassert>

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

template<size_t Size>
class Vector
{
private:
    std::array<double, Size> _data;

public:
    Vector() {};

    Vector(std::function<double (size_t)> const &generator)
        : _data(detail::fill<Size>(generator)) {};

    Vector(std::array<double, Size> &&values) : _data(values) {};

    Vector(std::initializer_list<double> const &values) {
        size_t index = 0;
        for (auto const &p : values) {
            assert(index < Size && "initializer list out of bound");
            _data[index++] = p;
        }
    };

    Vector(Vector const &v) : _data(v._data) {};
    Vector(Vector const &&v) : _data(std::move(v._data)) {};

    constexpr size_t size() {
        return Size;
    }

    void print() const {
        std::cout << std::setprecision(4) << '[' << std::endl;
        for (auto &d : _data) {
            std::cout << "  " << std::setw(10) << d << ',' << std::endl;
        }
        std::cout << ']' << std::endl;
    }

    double &operator ()(size_t i) {
        assert(i < Size && "vector.get: argument out of bound!");
        return _data[i];
    }

    double const &operator ()(size_t i) const {
        assert(i < Size && "vector.get: argument out of bound!");
        return _data[i];
    }

    Vector<Size> operator+(Vector<Size> const &v) const {
        Vector<Size> rv;
        for (int i = 0; i < Size; i++) {
            rv._data[i] = _data[i] + v._data[i];
        }
        std::cout << "adding" << std::endl;
        this->print();
        v.print();
        std::cout << "yields" << std::endl;
        rv.print();
        std::cout << "---" << std::endl;
        return rv;
    }

public:
    template<size_t M, size_t N> friend class Matrix;
};

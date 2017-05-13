#pragma once

#include <iostream>
#include <cstring>
#include <functional>
#include <iomanip>

template<int Size>
class Vector
{
private:
    double _data[Size];

public:
    Vector() {
        memset(_data, 0, Size * sizeof(double));
    }

    Vector(std::function<double (int)> const &generator) {
        for (int i = 0; i < Size; i++) {
            _data[i] = generator(i);
        }
    }

    constexpr int size() {
        return Size;
    }

    void print() {
        std::cout << std::setprecision(5) << '[' << std::endl;
        for (int i = 0; i < Size; i++) {
            std::cout << ' ' << _data[i] << ',' << std::endl;
        }
        std::cout << ']' << std::endl;
    }
};

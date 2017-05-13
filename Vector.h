#pragma once

#include <vector>

template<int Size>
class Vector
{
private:
    double _data[Size];

public:
    Vector();

    inline constexpr int size() {
        return Size;
    }
};

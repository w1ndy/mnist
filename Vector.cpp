#include "Vector.h"

#include <cstring>

template<int Size>
Vector::Vector() {
    memset(this._data, 0, sizeof(this._data));
}
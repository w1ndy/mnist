#pragma once

namespace detail
{
    template<typename T>
    struct SigmodActivator
    {
        static T run(T const &v) {
            return T([&v](int k) {
                return 1.0 / (1.0 + exp(-v(k)));
            });
        }
    };
}
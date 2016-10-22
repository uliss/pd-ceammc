#ifndef CEAMMC_HPP
#define CEAMMC_HPP

#include "ceammc.h"
#include <cmath>
#include <vector>

namespace ceammc {
namespace math {
    template <class T>
    static inline T ceil(T v);
    template <>
    float ceil(float f) { return ceilf(f); }
    template <>
    double ceil(double f) { return ceil(f); }
    template <>
    long double ceil(long double f) { return ceill(f); }

    template <class T>
    static inline bool is_natural(T v)
    {
        return (v >= 0 && (ceil<T>(v) == v));
    }
}

namespace pd {
    typedef std::vector<t_atom> atom_list;

    static inline void output(t_outlet* x, const atom_list& lst)
    {
        outlet_list(x, &s_list, static_cast<int>(lst.size()), const_cast<t_atom*>(lst.data()));
    }
}
}

#endif // CEAMMC_HPP

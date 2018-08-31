#ifndef CEAMMC_HPP
#define CEAMMC_HPP

#include "ceammc.h"
#include <cmath>
#include <cstring>
#include <functional>
#include <string>
#include <vector>

namespace ceammc {

std::vector<std::string> currentExtensionList();

std::string get_env(const char* varname);
void set_env(const char* varname, const char* val);

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

    template <class T>
    static inline bool is_integer(T v)
    {
        return ceil<T>(v) == v;
    }

    template <class T>
    static inline T clip(T v, T min, T max)
    {
        return std::max(min, std::min(v, max));
    }
}

namespace pd {
    static inline bool is_float(const t_atom& a)
    {
        return a.a_type == A_FLOAT || a.a_type == A_DEFFLOAT;
    }

    static inline bool is_symbol(const t_atom& a)
    {
        return a.a_type == A_DEFSYMBOL;
    }
}
}

#endif // CEAMMC_HPP

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
    typedef std::vector<t_atom> atom_list;
    bool operator==(const atom_list& l1, const atom_list& l2);
    bool operator!=(const atom_list& l1, const atom_list& l2);

    static inline bool is_float(const t_atom& a)
    {
        return a.a_type == A_FLOAT || a.a_type == A_DEFFLOAT;
    }

    static inline bool get_float(const t_atom& a, t_float* v)
    {
        if (is_float(a)) {
            *v = a.a_w.w_float;
            return true;
        }
        return false;
    }

    static inline bool is_symbol(const t_atom& a)
    {
        return a.a_type == A_DEFSYMBOL;
    }

    static inline void output(t_outlet* x, const atom_list& lst)
    {
        outlet_list(x, &s_list, static_cast<int>(lst.size()), const_cast<t_atom*>(lst.data()));
    }

    static bool atoms_compare_lt(const t_atom& a1, const t_atom& a2)
    {
        return ::ceammc_atoms_compare(&a1, &a2) == -1;
    }
}
}

#endif // CEAMMC_HPP

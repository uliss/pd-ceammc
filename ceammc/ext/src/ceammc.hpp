#ifndef CEAMMC_HPP
#define CEAMMC_HPP

#include "ceammc.h"
#include <cmath>
#include <vector>
#include <cstring>

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

    template <class T>
    static inline bool is_integer(T v)
    {
        return ceil<T>(v) == v;
    }
}

namespace pd {
    typedef std::vector<t_atom> atom_list;

    static inline void output(t_outlet* x, const atom_list& lst)
    {
        outlet_list(x, &s_list, static_cast<int>(lst.size()), const_cast<t_atom*>(lst.data()));
    }

    static bool atoms_compare_lt(const t_atom& a1, const t_atom& a2)
    {
        return ::ceammc_atoms_compare(&a1, &a2) == -1;
    }

    static bool atom_equals_to(const t_atom& a, t_float f)
    {
        return (a.a_type == A_FLOAT && a.a_w.w_float == f);
    }

    static bool atom_equals_to(const t_atom& a, t_symbol* s)
    {
        return (a.a_type == A_SYMBOL && a.a_w.w_symbol == s);
    }

    static bool atom_equals(const t_atom& a1, const t_atom& a2)
    {
        return memcmp(&a1, &a2, sizeof(a1)) == 0;
    }

    static bool atom_list_contains(const atom_list& lst, t_float v)
    {
        for (size_t i = 0; i < lst.size(); i++) {
            if (atom_equals_to(lst[i], v))
                return true;
        }
        return false;
    }

    static bool atom_list_contains(const atom_list& lst, t_symbol* s)
    {
        for (size_t i = 0; i < lst.size(); i++) {
            if (atom_equals_to(lst[i], s))
                return true;
        }
        return false;
    }

    static bool atom_list_contains(const atom_list& lst, const t_atom& a)
    {
        for (size_t i = 0; i < lst.size(); i++) {
            if (atom_equals(lst[i], a))
                return true;
        }
        return false;
    }
}
}

#endif // CEAMMC_HPP

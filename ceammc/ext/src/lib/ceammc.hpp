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

/**
 * math functions wrap
 */
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

// ====================

namespace pd {
    /**
     * @brief vector of pd t_atom objects
     */
    typedef std::vector<t_atom> atom_list;
/**
     * @brief operator == for the vector of pd t_atom objects
     * @param l1 first list
     * @param l2 second list
     * @return
     */
    bool operator==(const atom_list& l1, const atom_list& l2);
    /**
     * @brief operator != for the vector of pd t_atom objects
     * @param l1 first list
     * @param l2 second list
     * @return
     */
    bool operator!=(const atom_list& l1, const atom_list& l2);

    /**
     * @brief checks if t_atom is float
     * @param a
     * @return true if atom data type is float
     */
    static inline bool is_float(const t_atom& a)
    {
        return a.a_type == A_FLOAT || a.a_type == A_DEFFLOAT;
    }

    /**
     * @brief converts atom to float if avaible or returns false
     * @param a input atom
     * @param v output float value
     * @return true if successful, false otherwise
     */
    static inline bool get_float(const t_atom& a, t_float* v)
    {
        if (is_float(a)) {
            *v = a.a_w.w_float;
            return true;
        }
        return false;
    }

    /**
     * @brief checks if atom is symbol
     * @param a
     * @return
     */
    static inline bool is_symbol(const t_atom& a)
    {
        return a.a_type == A_DEFSYMBOL;
    }

    /**
     * @brief outpurs vector of t_atoms to a specific outlet
     * @param x
     * @param lst
     */
    static inline void output(t_outlet* x, const atom_list& lst)
    {
        outlet_list(x, &s_list, static_cast<int>(lst.size()), const_cast<t_atom*>(lst.data()));
    }

    /**
     * @brief compares if atom 1 is less than atom 2
     * @param a1 first atom
     * @param a2 second atom
     * @return
     */
    static bool atoms_compare_lt(const t_atom& a1, const t_atom& a2)
    {
        return ::ceammc_atoms_compare(&a1, &a2) == -1;
    }

    /**
     * @brief returns true if atom equals to float
     * @param a input
     * @param f value to compare with
     * @return
     */
    static bool atom_equals_to(const t_atom& a, t_float f)
    {
        return (a.a_type == A_FLOAT && a.a_w.w_float == f);
    }

    /**
     * @brief returns true if atom equals to symbol
     * @param a input
     * @param s symbol to compare with
     * @return
     */
    static bool atom_equals_to(const t_atom& a, t_symbol* s)
    {
        return (a.a_type == A_SYMBOL && a.a_w.w_symbol == s);
    }

    /**
     * @brief returns true if atom equals to another atom
     * @param a1 input
     * @param a2 atom to compare with
     * @return
     */
    static bool atom_equals(const t_atom& a1, const t_atom& a2)
    {
        return ceammc_atoms_equal(&a1, &a2);
    }

    /**
     * @brief returns true if list contains float value
     * @param lst imput list
     * @param v float value to find
     * @return
     */
    static bool atom_list_contains(const atom_list& lst, t_float v)
    {
        for (size_t i = 0; i < lst.size(); i++) {
            if (atom_equals_to(lst[i], v))
                return true;
        }
        return false;
    }

    /**
     * @brief returns true if list contains float value
     * @param lst imput list
     * @param v symbol to find
     * @return
     */
    static bool atom_list_contains(const atom_list& lst, t_symbol* s)
    {
        for (size_t i = 0; i < lst.size(); i++) {
            if (atom_equals_to(lst[i], s))
                return true;
        }
        return false;
    }

    /**
     * @brief returns true if list contains float value
     * @param lst imput list
     * @param v atom to find
     * @return
     */
    static bool atom_list_contains(const atom_list& lst, const t_atom& a)
    {
        for (size_t i = 0; i < lst.size(); i++) {
            if (atom_equals(lst[i], a))
                return true;
        }
        return false;
    }

    /**
     * @brief returns minimum and maximum values of floats within the input atom list
     * @param lst input atom list
     * @param min minimum value
     * @param max maximum value
     * @return true if succesful
     */
    bool atoms_minmax(const atom_list& lst, t_float* min, t_float* max);

    /**
     * @brief normalizes list
     * @param lst
     * @return true if succesful
     */
    bool atoms_normalize(atom_list& lst);
}
}

#endif // CEAMMC_HPP

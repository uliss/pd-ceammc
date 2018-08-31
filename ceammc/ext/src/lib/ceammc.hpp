#ifndef CEAMMC_HPP
#define CEAMMC_HPP

#include "ceammc.h"

#include <cmath>
#include <string>
#include <vector>

namespace ceammc {

std::vector<std::string> currentExtensionList();

std::string get_env(const char* varname);
void set_env(const char* varname, const char* val);

namespace math {
    template <class T>
    static inline bool is_natural(T v)
    {
        return (v >= 0 && (std::ceil(v) == v));
    }

    template <class T>
    static inline bool is_integer(T v)
    {
        return std::ceil(v) == v;
    }
}
}

#endif // CEAMMC_HPP

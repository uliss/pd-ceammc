/*****************************************************************************
 * Copyright 2021 Serge Poltavsky. All rights reserved.
 *
 * This file may be distributed under the terms of GNU Public License version
 * 3 (GPL v3) as defined by the Free Software Foundation (FSF). A copy of the
 * license should have been included with this file, or the project in which
 * this file belongs to. You may also find the details of GPL v3 at:
 * http://www.gnu.org/licenses/gpl-3.0.txt
 *
 * If you have any questions regarding the use of this file, feel free to
 * contact the author of this file, or the owner of the project in which
 * this file belongs to.
 *****************************************************************************/
#ifndef CEAMMC_NUI_H
#define CEAMMC_NUI_H

#include <cstddef>
#include <string>
#include <unordered_map>

#include "ceammc_nui_rect.h"

namespace ceammc {
namespace ui {

    template <class>
    struct hasher;
    template <>
    struct hasher<std::string> {
        std::size_t constexpr operator()(char const* input) const
        {
            return *input ? static_cast<unsigned int>(*input) + 33 * (*this)(input + 1) : 5381;
        }
        std::size_t operator()(const std::string& str) const
        {
            return (*this)(str.c_str());
        }
    };

    template <typename T>
    std::size_t constexpr hash(T&& t)
    {
        return hasher<typename std::decay<T>::type>()(std::forward<T>(t));
    }

    inline namespace literals {
        std::size_t constexpr operator"" _s(const char* s, size_t)
        {
            return hasher<std::string>()(s);
        }
    }

    struct Style {
        using Key = size_t;
        using HexColor = uint32_t;
        std::unordered_map<Key, HexColor> colors;
        std::unordered_map<Key, Size> min_sizes;
        std::unordered_map<Key, Size> max_sizes;
        std::unordered_map<Key, Size> default_sizes;

        bool hasColor(size_t key) const { return colors.find(key) != colors.end(); }
        bool getColor(size_t key, HexColor& c) const
        {
            auto it = colors.find(key);
            if (it == colors.end())
                return false;

            c = it->second;
            return true;
        }

        HexColor getBorderColor(HexColor def = 0xFFFFFFFF) const
        {
            getColor("border_color"_s, def);
            return def;
        }

        bool hasDefaultSize(size_t key) const { return default_sizes.find(key) != default_sizes.end(); }
        bool getDefaultSize(size_t key, Size& sz) const
        {
            auto it = default_sizes.find(key);
            if (it == default_sizes.end())
                return false;

            sz = it->second;
            return true;
        }
    };

}
}

#endif // CEAMMC_NUI_H

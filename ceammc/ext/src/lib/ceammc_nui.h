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

#include "ceammc_nui_color.h"
#include "ceammc_nui_font.h"
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
        std::size_t constexpr operator"" _hash(const char* s, size_t)
        {
            return hasher<std::string>()(s);
        }
    }

    class Style {
    public:
        using Key = size_t;

    private:
        std::unordered_map<Key, HexColor> colors;
        std::unordered_map<Key, Size> sizes;
        std::unordered_map<Key, Font> fonts;

    public:
        bool hasColor(Key key) const;
        bool getColor(Key key, HexColor& c) const;
        HexColor getColorWithDef(Key key, HexColor def) const;

        bool hasSize(Key key) const;
        bool getSize(Key key, Size& sz) const;
        Size getSizeWithDef(Key key, const Size& sz) const;

        bool hasFont(Key key) const;
        bool getFont(Key key, Font& ft) const;

        bool insertColor(Key key, HexColor c);
        bool insertSize(Key key, const Size& sz);
        bool insertFont(Key key, const Font& ft);
    };

}
}

#endif // CEAMMC_NUI_H

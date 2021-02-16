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
#include <vector>

#include "nui/color.h"
#include "nui/font.h"
#include "nui/rect.h"

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

    using StyleKey = size_t;

#define KEY_COLOR_BACKGROUND(el) hasher<std::string>()(#el ":bg")
#define KEY_COLOR_BORDER(el) hasher<std::string>()(#el ":bd")

    class Style {
        std::unordered_map<StyleKey, HexColor> colors;
        std::unordered_map<StyleKey, Size> sizes;
        std::unordered_map<StyleKey, Font> fonts;

    public:
        bool hasColor(StyleKey key) const;
        bool getColor(StyleKey key, HexColor& c) const;
        HexColor getColorWithDef(StyleKey key, HexColor def) const;

        bool hasSize(StyleKey key) const;
        bool getSize(StyleKey key, Size& sz) const;
        Size getSizeWithDef(StyleKey key, const Size& sz) const;

        bool hasFont(StyleKey key) const;
        bool getFont(StyleKey key, Font& ft) const;
        Font getFontWithDef(StyleKey key, const Font& ft) const;

        bool insertColor(StyleKey key, HexColor c);
        bool insertSize(StyleKey key, const Size& sz);
        bool insertFont(StyleKey key, const Font& ft);
    };

    class StyleCollection {
        StyleCollection(const StyleCollection&) = delete;
        StyleCollection& operator=(const StyleCollection&) = delete;

        StyleCollection();
        Style default_;
        std::vector<Style> styles_;

    public:
        static StyleCollection& instance();
        static const Style* style(int idx);
        static HexColor color(int styleIdx, StyleKey key, HexColor def);
        static Size size(int styleIdx, StyleKey key, const Size& def);
        static Font font(int styleIdx, StyleKey key, const Font& def);

        const Style* getStyle(int idx) const;
        HexColor getColor(int styleIdx, StyleKey key, HexColor def) const;
        Size getSize(int styleIdx, StyleKey key, const Size& def) const;
        Font getFont(int styleIdx, StyleKey key, const Font& def) const;
    };
}
}

#endif // CEAMMC_NUI_H

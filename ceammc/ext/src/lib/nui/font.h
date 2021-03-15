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
#ifndef CEAMMC_NUI_FONT_H
#define CEAMMC_NUI_FONT_H

#include <cstdint>
#include <cstring>

namespace ceammc {
namespace ui {

    enum class FontStyle : uint8_t {
        NORMAL,
        ITALIC
    };

    enum class FontWeight : uint8_t {
        NORMAL,
        BOLD
    };

    enum class FontVariant : uint8_t {
        NORMAL,
        SMALLCAPS
    };

    constexpr const char* FONT_FAMILY_DEFAULT = "Helvetica";
#ifdef __WIN32
    constexpr uint16_t FONT_SIZE_DEFAULT = 8;
#else
    constexpr uint16_t FONT_SIZE_DEFAULT = 10;
#endif

    class Font {
        static const uint32_t MAX_SIZE = 32;

    private:
        char family_[MAX_SIZE + 1] { 0 };
        uint16_t size_;
        FontStyle style_;
        FontWeight weight_;
        FontVariant variant_;

    public:
        Font(const char* family = FONT_FAMILY_DEFAULT,
            uint16_t size = FONT_SIZE_DEFAULT)
            : size_(size)
            , style_(FontStyle::NORMAL)
            , weight_(FontWeight::NORMAL)
            , variant_(FontVariant::NORMAL)
        {
            family_[0] = '\0';
            family_[MAX_SIZE] = '\0';
            std::strncpy(family_, family, MAX_SIZE);
        }

        const char* family() const { return family_; }
        int size() const { return size_; }
        const char* style() const { return style_ == FontStyle::ITALIC ? "italic" : "normal"; }
        const char* weight() const { return weight_ == FontWeight::BOLD ? "bold" : "normal"; }
        const char* variant() const { return variant_ == FontVariant::SMALLCAPS ? "smallcaps" : "normal"; }
    };

}
}

#endif // CEAMMC_NUI_FONT_H

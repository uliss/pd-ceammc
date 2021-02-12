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
#include <string>

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
        SMALL_CAPS
    };

    constexpr const char* FONT_FAMILY_DEFAULT = "Helvetica";

    struct Font {
        std::string family { FONT_FAMILY_DEFAULT };
        uint16_t size { 8 };
        FontStyle style { FontStyle::NORMAL };
        FontWeight weight { FontWeight::NORMAL };
        FontVariant variant { FontVariant::NORMAL };
    };

}
}

#endif // CEAMMC_NUI_FONT_H

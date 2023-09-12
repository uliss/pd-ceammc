/*****************************************************************************
 * Copyright 2023 Serge Poltavski. All rights reserved.
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
#ifndef CEAMMC_FONTS_H
#define CEAMMC_FONTS_H

namespace ceammc {
namespace fonts {

    constexpr float DEFAULT_MACOS_MENU_FONT_SIZE = 11;
    constexpr float DEFAULT_LINUX_MENU_FONT_SIZE = 11;
    constexpr float DEFAULT_WINDOWS_MENU_FONT_SIZE = 7;

    constexpr const char* DEFAULT_MACOS_MENU_FONT_FAMILY = "systemSmallSystemFont";
    constexpr const char* DEFAULT_LINUX_MENU_FONT_FAMILY = "Helvetica";
    constexpr const char* DEFAULT_WINDOWS_MENU_FONT_FAMILY = "Dejavu Sans";

    static inline float default_menu_font_size()
    {
#ifdef __APPLE__
        return DEFAULT_MACOS_MENU_FONT_SIZE;
#elif defined(__WIN32)
        return DEFAULT_WINDOWS_MENU_FONT_SIZE;
#else
        return DEFAULT_LINUX_MENU_FONT_SIZE;
#endif
    }

    static inline constexpr const char* default_menu_font_family()
    {
#ifdef __APPLE__
        return DEFAULT_MACOS_MENU_FONT_FAMILY;
#elif defined(__WIN32)
        return DEFAULT_WINDOWS_MENU_FONT_FAMILY;
#else
        return DEFAULT_LINUX_MENU_FONT_FAMILY;
#endif
    }

}
}

#endif // CEAMMC_FONTS_H

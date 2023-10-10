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
#ifndef ESYMBOLS_H
#define ESYMBOLS_H

#include "ceammc_crc32.h"
#include "ceammc_symbols.h"

#define CICM_DEFINE_SYMBOL2(name, value) \
    CEAMMC_DEFINE_SYMBOL2(name, value)   \
    constexpr const auto hash_##name = value##_hash;

#define CICM_DEFINE_SYMBOL(name) CICM_DEFINE_SYMBOL2(name, #name)

#define CICM_DEFINE_METHOD2(name, value)                         \
    CEAMMC_DEFINE_METHOD2(name, value)                           \
    namespace methods {                                          \
    constexpr auto hash_##name = ceammc::crc32_constexpr(value); \
    }

#define CICM_DEFINE_METHOD(name) CICM_DEFINE_METHOD2(name, #name)

#define CICM_DEFINE_PROP(name) \
    CEAMMC_DEFINE_PROPERTY2(name, #name)

namespace ceammc {
namespace sym {
    CICM_DEFINE_METHOD2(widget_create, ".create")
    CICM_DEFINE_METHOD2(widget_erase, ".erase")

    // mouse
    CICM_DEFINE_METHOD(dblclick)
    CICM_DEFINE_METHOD(mousedown)
    CICM_DEFINE_METHOD(mousedrag)
    CICM_DEFINE_METHOD(mouseenter)
    CICM_DEFINE_METHOD(mouseleave)
    CICM_DEFINE_METHOD(mousemove)
    CICM_DEFINE_METHOD(mouseup)
    CICM_DEFINE_METHOD(mousewheel)
    CICM_DEFINE_METHOD(rightclick)

    CICM_DEFINE_METHOD(key)
    CICM_DEFINE_METHOD(keyfilter)

    // ui
    CICM_DEFINE_METHOD(dialog)
    CICM_DEFINE_METHOD(getdrawparams)
    CICM_DEFINE_METHOD(notify)
    CICM_DEFINE_METHOD(oksize)
    CICM_DEFINE_METHOD(onzoom)
    CICM_DEFINE_METHOD(paint)
    CICM_DEFINE_METHOD(popup)
    CICM_DEFINE_METHOD(preset)
    CICM_DEFINE_METHOD(save)
    CICM_DEFINE_METHOD(size)

    // widgets
    CICM_DEFINE_SYMBOL(checkbutton)
    CICM_DEFINE_SYMBOL(entry)
    CICM_DEFINE_SYMBOL(menu)
    CICM_DEFINE_SYMBOL(onoff)

    CICM_DEFINE_SYMBOL(color)
    CICM_DEFINE_SYMBOL(number)
    CICM_DEFINE_SYMBOL(path)
    CICM_DEFINE_SYMBOL(rgb)
    CICM_DEFINE_SYMBOL(rgba)
    CICM_DEFINE_SYMBOL(spinbox)

    // fonts
    CICM_DEFINE_SYMBOL(bold)
    CICM_DEFINE_SYMBOL(italic)
    CICM_DEFINE_SYMBOL(normal)
    CICM_DEFINE_SYMBOL(roman)

    // pd
    CEAMMC_DEFINE_CRC32(anything)
    CEAMMC_DEFINE_CRC32(bang)
    CEAMMC_DEFINE_CRC32(dsp)
    CEAMMC_DEFINE_CRC32(float)
    CEAMMC_DEFINE_CRC32(list)
    CEAMMC_DEFINE_CRC32(symbol)
    CICM_DEFINE_SYMBOL(double)
    CICM_DEFINE_SYMBOL(int)
    CICM_DEFINE_SYMBOL(long)

    // cicm
    CICM_DEFINE_SYMBOL2(null, "(null)");
    CICM_DEFINE_SYMBOL2(iscicm, ".is_cicm?");
    CICM_DEFINE_SYMBOL2(color_black_hex, "#000000");

    CICM_DEFINE_SYMBOL(atom)
    CICM_DEFINE_SYMBOL(attr_modified)
    CICM_DEFINE_SYMBOL(obj)
    CICM_DEFINE_SYMBOL(pinned)

    // props
    CICM_DEFINE_PROP(fontsize)
    CICM_DEFINE_PROP(fontslant)
    CICM_DEFINE_PROP(fontweight)
    CICM_DEFINE_PROP(label)
    CICM_DEFINE_PROP(label_align)
    CICM_DEFINE_PROP(label_inner)
    CICM_DEFINE_PROP(label_side)
    CICM_DEFINE_PROP(label_valign)
    CICM_DEFINE_PROP(presetname)
    CICM_DEFINE_PROP(size)

    // colors
    CICM_DEFINE_PROP(active_color)
    CICM_DEFINE_PROP(background_color)
    CICM_DEFINE_PROP(border_color)
    CICM_DEFINE_PROP(label_color)
    CICM_DEFINE_PROP(text_color)

    CICM_DEFINE_SYMBOL2(label_align_left, "left")
    CICM_DEFINE_SYMBOL2(label_align_center, "center")
    CICM_DEFINE_SYMBOL2(label_align_right, "right")

    CICM_DEFINE_SYMBOL2(label_valign_top, "top")
    CICM_DEFINE_SYMBOL2(label_valign_center, "center")
    CICM_DEFINE_SYMBOL2(label_valign_bottom, "bottom")

    CICM_DEFINE_SYMBOL2(label_side_left, "left")
    CICM_DEFINE_SYMBOL2(label_side_right, "right")
    CICM_DEFINE_SYMBOL2(label_side_top, "top")
    CICM_DEFINE_SYMBOL2(label_side_bottom, "bottom")
}
}

#endif // ESYMBOLS_H

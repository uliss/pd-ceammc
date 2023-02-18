/*****************************************************************************
 * Copyright 2023 Serge Poltavsky. All rights reserved.
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
#ifndef UI_LCD_H
#define UI_LCD_H

#include "ceammc_ui_object.h"

#include <boost/dynamic_bitset.hpp>
#include <cstdint>

namespace ceammc {

using Pixels = boost::dynamic_bitset<std::uint32_t>;

class UILcd : public UIObject {
private:
    t_rgba prop_color_active;
    int prop_ncols, prop_nrows, pixel_size;
    Pixels pixels_;

public:
    UILcd();

    void init(t_symbol* name, const AtomListView& args, bool usePresets);
    void okSize(t_rect* newrect);
    void paint();

    void onAny(t_symbol* s, const AtomListView&);

    void m_set(const AtomListView& lv);
    void m_clear();
    void m_invert(const AtomListView& lv);

    t_float p_numCols() const { return prop_ncols; }
    void p_setNumCols(t_float n);

    t_float p_numRows() const { return prop_nrows; }
    void p_setNumRows(t_float n);

public:
    static void setup();

private:
    std::uint32_t pixelIndex(std::uint32_t x, std::uint32_t y) const
    {
        return x + prop_ncols * y;
    }

    bool pixelAt(std::uint32_t x, std::uint32_t y) const
    {
        auto pos = pixelIndex(x, y);
        return pos >= pixels_.size() ? false : pixels_.test(pos);
    }
};

}

void setup_ui_lcd();

#endif // UI_LCD_H

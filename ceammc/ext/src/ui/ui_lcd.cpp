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
#include "ui_lcd.h"
#include "ceammc_crc32.h"
#include "ceammc_ui.h"
#include "fmt/core.h"
#include "ui_lcd.tcl.h"

constexpr int DEF_PIXEL_SIZE = 8;
constexpr int DEF_XPIXELS = 8;
constexpr int DEF_YPIXELS = 8;
constexpr int MAX_PIXEL_SIZE = 48;
constexpr int MAX_XPIXELS = 128;
constexpr int MAX_YPIXELS = 128;
constexpr int MIN_PIXEL_SIZE = 1;
constexpr int MIN_XPIXELS = 2;
constexpr int MIN_YPIXELS = 2;

namespace ceammc {

UILcd::UILcd()
    : prop_color_active(rgba_blue)
    , prop_x(DEF_XPIXELS)
    , prop_y(DEF_YPIXELS)
{
    createOutlet();
}

void UILcd::init(t_symbol* name, const AtomListView& args, bool usePresets)
{
    UIObject::init(name, args, usePresets);

    pixels_.resize(prop_x * prop_y);
}

void UILcd::okSize(t_rect* newrect)
{
    newrect->width = prop_x * std::round(pd_clip_min(newrect->width / prop_x, prop_x * MIN_XPIXELS));
    newrect->height = prop_y * std::round(pd_clip_min(newrect->height / prop_y, prop_y * MIN_YPIXELS));
    pixel_size = newrect->width / prop_x;
}

void UILcd::paint()
{
    sys_vgui("::ui::lcd_update %s %lx"
             " %d %d %d"
             " %d %d %d",
        asEBox()->b_canvas_id->s_name, asEBox(),
        (int)width(), (int)height(), (int)zoom(),
        prop_x, prop_y, pixel_size);

    auto bits = sizeof(std::uint32_t) * 8;

    for (size_t i = 0; i < pixels_.size(); i += bits) {
        std::uint32_t n = 0;
        for (size_t b = 0; b < bits; b++) {
            auto idx = i + b;
            if (idx >= pixels_.size())
                goto end;
            else if (pixels_.test(idx))
                n |= (1 << b);
        }

        sys_vgui(" %#08X", n);
    }

end:
    sys_vgui("\n");
}

void UILcd::m_set(const AtomListView& lv)
{
    if (lv.size() < 3) {
        UI_ERR << "usage: set [pixel|col|row]? ARGS...";
        return;
    }

    auto sel = crc32_hash(lv.symbolAt(0, &s_));

    switch (sel) {
    case "pixel"_hash:
        pixels_.set(pixelIndex(lv.intAt(1, 0), lv.intAt(2, 0)), lv.boolAt(3, false));
        break;
    case "row"_hash: {
        // set row IDX 0 1 1 0 1
        const auto ROW_IDX = lv.intAt(1, -1);
        if (ROW_IDX < 0 || ROW_IDX >= prop_y) {
            UI_ERR << fmt::format("invalid row value, expected in [0,{}) range, got: ", prop_y, ROW_IDX);
            return;
        }

        for (int i = 0; i < std::min<int>(lv.size() - 2, prop_x); i++) {
            auto idx = pixelIndex(i, ROW_IDX);
            if (idx >= pixels_.size())
                break;

            pixels_.set(idx, lv.boolAt(i + 2, false));
        }
    } break;
    case "col"_hash: {
        // set col IDX 0 1 1 0 1
        const auto COL_IDX = lv.intAt(1, -1);
        if (COL_IDX < 0 || COL_IDX >= prop_x) {
            UI_ERR << fmt::format("invalid column value, expected in [0,{}) range, got: ", prop_x, COL_IDX);
            return;
        }

        for (int i = 0; i < std::min<int>(lv.size() - 2, prop_y); i++) {
            auto idx = pixelIndex(COL_IDX, i);
            if (idx >= pixels_.size())
                break;

            pixels_.set(idx, lv.boolAt(i + 2, false));
        }
    } break;
    }

    redrawBGLayer();
}

void UILcd::m_clear()
{
    pixels_.reset();
    redrawBGLayer();
}

void UILcd::setup()
{
    sys_gui(ui_lcd_tcl);

    UIObjectFactory<UILcd> obj("ui.lcd", EBOX_GROWINDI);

    obj.setDefaultSize(DEF_XPIXELS * DEF_PIXEL_SIZE, DEF_YPIXELS * DEF_PIXEL_SIZE);
    obj.addIntProperty("x", _("X"), DEF_XPIXELS, &UILcd::prop_x);
    obj.addIntProperty("y", _("Y"), DEF_YPIXELS, &UILcd::prop_y);
    obj.setPropertyRange("x", MIN_XPIXELS, MAX_XPIXELS);
    obj.setPropertyRange("y", MIN_YPIXELS, MAX_YPIXELS);

    obj.addMethod("set", &UILcd::m_set);
    obj.addMethod("clear", &UILcd::m_clear);
}

}

void setup_ui_lcd()
{
    ceammc::UILcd::setup();
}

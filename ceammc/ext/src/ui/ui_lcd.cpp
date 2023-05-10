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
#include "ceammc_convert.h"
#include "ceammc_crc32.h"
#include "ceammc_ui.h"
#include "fmt/core.h"
#include "ui_lcd.tcl.h"

constexpr int DEF_COLS = 8;
constexpr int MAX_COLS = 128;
constexpr int MIN_COLS = 2;

constexpr int DEF_PIXEL_SIZE = 8;
constexpr int MAX_PIXEL_SIZE = 48;
constexpr int MIN_PIXEL_SIZE = 1;

constexpr int DEF_ROWS = 8;
constexpr int MAX_ROWS = 128;
constexpr int MIN_ROWS = 2;

namespace ceammc {

UILcd::UILcd()
    : prop_color_active(rgba_blue)
    , prop_ncols(DEF_COLS)
    , prop_nrows(DEF_ROWS)
    , pixel_size(DEF_PIXEL_SIZE)
{
    createOutlet();
}

void UILcd::init(t_symbol* name, const AtomListView& args, bool usePresets)
{
    UIObject::init(name, args, usePresets);

    pixels_.resize(prop_ncols * prop_nrows);
}

void UILcd::okSize(t_rect* newrect)
{
    int xp = std::round(pd_clip_minmax(newrect->w / prop_ncols, MIN_PIXEL_SIZE, MAX_PIXEL_SIZE));
    int yp = std::round(pd_clip_minmax(newrect->h / prop_nrows, MIN_PIXEL_SIZE, MAX_PIXEL_SIZE));

    if (prop_nrows < prop_ncols)
        pixel_size = xp;
    else
        pixel_size = yp;

    newrect->w = prop_ncols * pixel_size;
    newrect->h = prop_nrows * pixel_size;
}

void UILcd::paint()
{
    sys_vgui("::ui::lcd_update %s %lx"
             " %d %d"
             " %d %d %d",
        asEBox()->b_canvas_id->s_name, asEBox(),
        (int)width(), (int)height(),
        prop_ncols, prop_nrows, (int)(pixel_size * zoom()));

    const auto NBITS = sizeof(std::uint32_t) * 8;
    const auto NPIX = pixels_.size();
    auto num_args = NPIX / NBITS;
    if (NPIX % NBITS != 0)
        num_args++;

    for (size_t i = 0; i < num_args; i++) {
        std::uint32_t n = 0;
        for (size_t b = 0; b < NBITS; b++) {
            auto idx = i * NBITS + b;
            if (idx >= pixels_.size())
                break;
            else if (pixels_.test(idx))
                n |= (1 << b);
        }

        sys_vgui(" %#x", n);
    }

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
        if (ROW_IDX < 0 || ROW_IDX >= prop_nrows) {
            UI_ERR << fmt::format("invalid row value, expected in [0,{}) range, got: ", prop_nrows, ROW_IDX);
            return;
        }

        for (int i = 0; i < std::min<int>(lv.size() - 2, prop_ncols); i++) {
            auto idx = pixelIndex(i, ROW_IDX);
            if (idx >= pixels_.size())
                break;

            pixels_.set(idx, lv.boolAt(i + 2, false));
        }
    } break;
    case "col"_hash: {
        // set col IDX 0 1 1 0 1
        const auto COL_IDX = lv.intAt(1, -1);
        if (COL_IDX < 0 || COL_IDX >= prop_ncols) {
            UI_ERR << fmt::format("invalid column value, expected in [0,{}) range, got: ", prop_ncols, COL_IDX);
            return;
        }

        for (int i = 0; i < std::min<int>(lv.size() - 2, prop_nrows); i++) {
            auto idx = pixelIndex(COL_IDX, i);
            if (idx >= pixels_.size())
                break;

            pixels_.set(idx, lv.boolAt(i + 2, false));
        }
    } break;
    default: {
        auto N = pixels_.size();
        for (size_t i = 0; i < std::min(lv.size(), N); i++) {
            auto row = i / prop_ncols;
            auto col = i % prop_ncols;
            auto idx = pixelIndex(col, row);
            if (idx >= N)
                break;

            pixels_.set(idx, lv.boolAt(i, false));
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

void UILcd::m_invert(const AtomListView& lv)
{
    if (lv.empty()) {
        pixels_.flip();
        redrawBGLayer();
        return;
    }

    auto sel = crc32_hash(lv.symbolAt(0, &s_));

    switch (sel) {
    case "pixel"_hash:
        pixels_.flip(pixelIndex(lv.intAt(1, 0), lv.intAt(2, 0)));
        break;
    default:
        pixels_.flip();
        break;
    }

    redrawBGLayer();
}

void UILcd::p_setNumCols(t_int n)
{
    int num = clip<int>(n, MIN_COLS, MAX_COLS);
    if (num != prop_ncols) {
        prop_ncols = num;
        pixels_.resize(prop_ncols * prop_nrows);
        updateSize();
    }
}

void UILcd::p_setNumRows(t_int n)
{
    int num = clip<int>(n, MIN_ROWS, MAX_ROWS);
    if (num != prop_nrows) {
        prop_nrows = num;
        pixels_.resize(prop_ncols * prop_nrows);
        updateSize();
    }
}

void UILcd::setup()
{
    sys_gui(ui_lcd_tcl);

    UIObjectFactory<UILcd> obj("ui.lcd", EBOX_GROWLINK);

    obj.setPropertyDefaultValue("background_color", "1 1 1 1");

    obj.setDefaultSize(DEF_COLS * DEF_PIXEL_SIZE, DEF_ROWS * DEF_PIXEL_SIZE);
    obj.addIntProperty("cols", _("Number of columns"), DEF_COLS, &UILcd::prop_ncols);
    obj.setPropertyRange("cols", MIN_COLS, MAX_COLS);
    obj.setPropertyAccessor("cols", &UILcd::p_numCols, &UILcd::p_setNumCols);

    obj.addIntProperty("rows", _("Number of rows"), DEF_ROWS, &UILcd::prop_nrows);
    obj.setPropertyRange("rows", MIN_ROWS, MAX_ROWS);
    obj.setPropertyAccessor("rows", &UILcd::p_numRows, &UILcd::p_setNumRows);

    obj.addHiddenIntCbProperty("pixels", &UILcd::p_pixels, nullptr);

    obj.addMethod("set", &UILcd::m_set);
    obj.addMethod("clear", &UILcd::m_clear);
    obj.addMethod("invert", &UILcd::m_invert);
}

}

void setup_ui_lcd()
{
    ceammc::UILcd::setup();
}

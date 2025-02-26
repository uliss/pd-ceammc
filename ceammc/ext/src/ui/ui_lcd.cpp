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
#include "args/argcheck.h"
#include "ceammc_convert.h"
#include "ceammc_crc32.h"
#include "ceammc_ui.h"
#include "cicm/Sources/egraphics.h"
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

#define CHECK_UI_ARGS(chk, lv, method)                      \
    {                                                       \
        if (!chk.check_pd_obj(lv, asPdObject()))            \
            return chk.usage(asPdObject(), gensym(method)); \
    }

namespace ceammc {

CEAMMC_DEFINE_HASH(col)
CEAMMC_DEFINE_HASH(cursor)
CEAMMC_DEFINE_HASH(list)
CEAMMC_DEFINE_HASH(matrix)
CEAMMC_DEFINE_HASH(pixel)
CEAMMC_DEFINE_HASH(row)

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

void ceammc::UILcd::setCursor(const AtomListView& lv)
{
    static const args::ArgChecker chk("X:i Y:i");
    CHECK_UI_ARGS(chk, lv, "cursor");

    cursor_.x = lv.intAt(0, 0);
    cursor_.y = lv.intAt(1, 0);
}

void UILcd::setList(const AtomListView& lv)
{
    // set list 1 0 0 1
    auto N = pixels_.size();
    for (size_t i = 0; i < std::min(lv.size(), N); i++) {
        auto row = i / prop_ncols;
        auto col = i % prop_ncols;
        auto idx = pixelIndex(col, row);
        if (idx >= N)
            break;

        pixels_.set(idx, lv.boolAt(i, false));
    }
}

void ceammc::UILcd::setPixel(const AtomListView& lv)
{
    // set pixel X Y 1
    static const args::ArgChecker chk("X:i Y:i VAL:B");
    CHECK_UI_ARGS(chk, lv, "pixel");

    pixels_.set(pixelIndex(lv.intAt(0, 0), lv.intAt(1, 0)), lv.boolAt(2, false));
}

void UILcd::setCol(const AtomListView& lv)
{
    // set col IDX 0 1 1 0 1
    static const args::ArgChecker chk("IDX:i>=0 DATA:B+");
    CHECK_UI_ARGS(chk, lv, "col");

    const auto col = lv.intAt(0, 0);
    const auto data = lv.subView(1);

    if (col >= prop_ncols) {
        UI_ERR << fmt::format("invalid column value, expected in [0,{}) range, got: ", prop_ncols, col);
        return;
    }

    for (int i = 0; i < std::min<int>(data.size(), prop_ncols); i++) {
        auto idx = pixelIndex(col, i);
        if (idx >= pixels_.size())
            break;

        pixels_.set(idx, data.boolAt(i, false));
    }
}

void UILcd::setRow(const AtomListView& lv)
{
    // set row IDX 0 1 1 0 1
    static const args::ArgChecker chk("IDX:i>=0 DATA:B+");
    CHECK_UI_ARGS(chk, lv, "row");

    const auto row = lv.intAt(0, 0);
    const auto data = lv.subView(1);

    if (row >= prop_nrows) {
        UI_ERR << fmt::format("invalid row value, expected in [0,{}) range, got: ", prop_nrows, row);
        return;
    }

    for (int i = 0; i < std::min<int>(data.size(), prop_nrows); i++) {
        auto idx = pixelIndex(i, row);
        if (idx >= pixels_.size())
            break;

        pixels_.set(idx, data.boolAt(i, false));
    }
}

void UILcd::setMatrix(const AtomListView& lv)
{
    // set matrix NROWS NCOLS 0 1 1 0 1
    static const args::ArgChecker chk("NROWS:i>=0 NCOLS:i>=0 DATA:B+");
    CHECK_UI_ARGS(chk, lv, "matrix");

    auto nrows = lv.intAt(0, 0);
    auto ncols = lv.intAt(1, 0);
    auto data = lv.subView(2);

    for (int r = 0; r < std::min(nrows, prop_nrows); r++) {
        for (int c = 0; c < std::min(ncols, prop_ncols); c++) {
            auto idx = r * ncols + c;
            if (idx >= data.size())
                continue;

            auto val = data.boolAt(idx, false);
            pixels_.set(pixelIndex(c, r), val);
        }
    }
}

void UILcd::m_set(const AtomListView& lv)
{
    static const args::ArgChecker chk("s=list|pixel|col|row|cursor|matrix DATA:a*");
    CHECK_UI_ARGS(chk, lv, "set");

    auto sel = lv.symbolAt(0, &s_);
    auto data = lv.subView(1);

    switch (crc32_hash(sel)) {
    case hash_cursor:
        setCursor(data);
        break;
    case hash_pixel:
        setPixel(data);
        break;
    case hash_row:
        setRow(data);
        break;
    case hash_col:
        setCol(data);
        break;
    case hash_matrix:
        setMatrix(data);
        break;
    default:
        setList(data);
        break;
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

void UILcd::m_draw(const AtomListView& lv)
{
    static const args::ArgChecker chk_set("s=set VALUE:b");
    static const args::ArgChecker chk_up("s=up N:i>0?");
    static const args::ArgChecker chk_down("s=down N:i>0?");
    static const args::ArgChecker chk_left("s=left N:i>0?");
    static const args::ArgChecker chk_right("s=right N:i>0?");

    if (chk_set.check(lv, nullptr, nullptr, false)) {
        draw_value_ = lv.boolAt(1, true);
        return;
    } else if (chk_up.check(lv, nullptr, nullptr, false)) {
        moveCursorY(-lv.intAt(1, 1));
        drawCursor();
    } else if (chk_down.check(lv, nullptr, nullptr, false)) {
        moveCursorY(lv.intAt(1, 1));
        drawCursor();
    } else if (chk_left.check(lv, nullptr, nullptr, false)) {
        moveCursorX(-lv.intAt(1, 1));
        drawCursor();
    } else if (chk_right.check(lv, nullptr, nullptr, false)) {
        moveCursorX(lv.intAt(1, 1));
        drawCursor();
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
    ui_lcd_tcl_output();

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
    obj.addMethod("draw", &UILcd::m_draw);
}

}

void setup_ui_lcd()
{
    ceammc::UILcd::setup();
}

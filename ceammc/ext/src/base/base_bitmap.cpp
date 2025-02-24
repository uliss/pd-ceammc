/*****************************************************************************
 * Copyright 2025 Serge Poltavski. All rights reserved.
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
#include "base_bitmap.h"
#include "args/argcheck.h"
#include "ceammc_factory.h"

using namespace ceammc;

BaseBitmap::BaseBitmap(const PdArgs& args)
    : DispatchedObject<BaseObject>(args)
{
    w_ = new IntProperty("@w", 8);
    w_->checkClosedRange(1, 256);
    w_->setInitOnly();
    w_->setArgIndex(0);
    addProperty(w_);

    h_ = new IntProperty("@h", 8);
    h_->setInitOnly();
    h_->checkClosedRange(1, 256);
    h_->setArgIndex(1);
    addProperty(h_);

    createOutlet();
}

BaseBitmap::~BaseBitmap()
{
    ceammc_bitmap_free(bm_);
}

void BaseBitmap::initDone()
{
    bm_ = ceammc_bitmap_new(w_->value(), h_->value(),
        { subscriberId(), [](size_t id) { Dispatcher::instance().send({ id, 0 }); } },
        { this, [](void* user, std::uint16_t rows, std::uint16_t cols, ceammc_core_bitmap_output_format fmt, const std::uint8_t* data, size_t len) {
             auto obj = static_cast<BaseBitmap*>(user);
             if (!obj)
                 return;

             AtomList res;
             switch (fmt) {
             case ceammc_core_bitmap_output_format::LIST:
                 res.reserve(len);
                 break;
             case ceammc_core_bitmap_output_format::MATRIX:
                 res.reserve(len + 2);
                 res.push_back(rows);
                 res.push_back(cols);
                 break;
             }

             res.reserve(len);
             for (size_t i = 0; i < len; i++) {
                 res.push_back(data[i]);
             }

             switch (fmt) {
             case ceammc_core_bitmap_output_format::LIST:
                 obj->listTo(0, res);
                 break;
             case ceammc_core_bitmap_output_format::MATRIX:
                 obj->anyTo(0, gensym("matrix"), res);
                 break;
             }
         } },
        { this, [](void* user, const char* msg) {
             auto obj = static_cast<BaseBitmap*>(user);
             if (!obj)
                 return;

             Error(obj) << msg;
         } });
}

bool BaseBitmap::notify(int code)
{
    ceammc_bitmap_process(bm_);
    return true;
}

void BaseBitmap::onBang()
{
    ceammc_bitmap_get_data(bm_);
}

void BaseBitmap::m_arc(t_symbol* s, const AtomListView& lv)
{
    static const args::ArgChecker chk("X:i>=0 Y:i>=0 DIAM:i>0 ANGLE:f LEN:f CENTER:B?");
    if (!chk.check(lv, this)) {
        return chk.usage(this, s);
    }

    auto x = lv.intAt(0, 0);
    auto y = lv.intAt(1, 0);
    auto diam = lv.intAt(2, 2);
    auto start = lv.floatAt(3, 2);
    auto length = lv.floatAt(4, 2);
    auto center = lv.boolAt(5, false);

    ceammc_bitmap_draw_arc(bm_, x, y, diam, start, length, center);
}

void BaseBitmap::m_clear(t_symbol* s, const AtomListView& lv)
{
    ceammc_bitmap_clear(bm_);
}

void BaseBitmap::m_column(t_symbol* s, const AtomListView& lv)
{
    static const args::ArgChecker chk("COL:i>=0 HT:i DY:i?");
    if (!chk.check(lv, this)) {
        return chk.usage(this, s);
    }

    auto col = lv.intAt(0, 0);
    auto ht = lv.intAt(1, 0);
    auto dy = lv.intAt(2, 0);

    ceammc_bitmap_draw_column(bm_, col, ht, dy);
}

void BaseBitmap::m_row(t_symbol* s, const AtomListView& lv)
{
    static const args::ArgChecker chk("ROW:i>=0 HT:i DY:i?");
    if (!chk.check(lv, this)) {
        return chk.usage(this, s);
    }

    auto row = lv.intAt(0, 0);
    auto wd = lv.intAt(1, 0);
    auto dx = lv.intAt(2, 0);

    ceammc_bitmap_draw_row(bm_, row, wd, dx);
}

void BaseBitmap::m_text(t_symbol* s, const AtomListView& lv)
{
    auto x = lv.intAt(0, 0);
    auto y = lv.intAt(1, 0);
    ceammc_bitmap_draw_text(bm_, lv.symbolAt(2, &s_)->s_name, x, y);
}

void BaseBitmap::m_triangle(t_symbol* s, const AtomListView& lv)
{
    static const args::ArgChecker chk("X0:i Y0:i X1:i Y1:i X2:i Y2:i");
    if (!chk.check(lv, this)) {
        return chk.usage(this, s);
    }

    auto x0 = lv.intAt(0, 0);
    auto y0 = lv.intAt(1, 0);
    auto x1 = lv.intAt(2, 0);
    auto y1 = lv.intAt(3, 0);
    auto x2 = lv.intAt(4, 0);
    auto y2 = lv.intAt(5, 0);

    ceammc_bitmap_draw_triangle(bm_, x0, y0, x1, y1, x2, y2);
}

void BaseBitmap::m_pixel(t_symbol* s, const AtomListView& lv)
{
    static const args::ArgChecker chk("X:i>=0 Y:i>=0 VALUE:B?");
    if (!chk.check(lv, this)) {
        return chk.usage(this, s);
    }

    auto x = lv.intAt(0, 0);
    auto y = lv.intAt(1, 0);
    auto value = lv.boolAt(2, true);

    ceammc_bitmap_draw_pixel(bm_, x, y, value);
}

void BaseBitmap::m_circle(t_symbol* s, const AtomListView& lv)
{
    static const args::ArgChecker chk("X:i>=0 Y:i>=0 DIAM:i>0 CENTER:B?");
    if (!chk.check(lv, this)) {
        return chk.usage(this, s);
    }

    auto x = lv.intAt(0, 0);
    auto y = lv.intAt(1, 0);
    auto diam = lv.intAt(2, 2);
    auto center = lv.boolAt(3, false);

    ceammc_bitmap_draw_circle(bm_, x, y, diam, center);
}

void BaseBitmap::m_ellipse(t_symbol* s, const AtomListView& lv)
{
    static const args::ArgChecker chk("X:i>=0 Y:i>=0 W:i>0 H:i>0 CENTER:B?");
    if (!chk.check(lv, this)) {
        return chk.usage(this, s);
    }

    auto x = lv.intAt(0, 0);
    auto y = lv.intAt(1, 0);
    auto w = lv.intAt(2, 0);
    auto h = lv.intAt(3, 0);
    auto center = lv.boolAt(4, false);

    ceammc_bitmap_draw_ellipse(bm_, x, y, w, h, center);
}

void BaseBitmap::m_vshift(t_symbol* s, const AtomListView& lv)
{
    static const args::ArgChecker chk("DY:i");
    if (!chk.check(lv, this)) {
        return chk.usage(this, s);
    }

    ceammc_bitmap_vshift(bm_, lv.intAt(0, 0));
}

void BaseBitmap::m_get(t_symbol* s, const AtomListView& lv)
{
    onBang();
}

void BaseBitmap::m_get_matrix(t_symbol* s, const AtomListView& lv)
{
    ceammc_bitmap_get_matrix(bm_);
    // outputs matrix NROWS NCOLS DATA...
}

void BaseBitmap::m_get_submatrix(t_symbol* s, const AtomListView& lv)
{
    static const args::ArgChecker chk("ROW:i>=0 COL:i>=0 NROWS:i>=0 NCOLS:i>=0");
    if (!chk.check(lv, this)) {
        return chk.usage(this, s);
    }

    auto r = lv.intAt(0, 0);
    auto c = lv.intAt(1, 0);
    auto nrows = lv.intAt(2, 0);
    auto ncols = lv.intAt(3, 0);

    ceammc_bitmap_get_submatrix(bm_, r, c, nrows, ncols);
    // outputs matrix NROWS NCOLS DATA...
}

void BaseBitmap::m_set(t_symbol* s, const AtomListView& lv)
{
    std::vector<std::uint8_t> bytes;
    bytes.reserve(lv.size());
    for (auto& a : lv)
        bytes.push_back(a.asInt());

    ceammc_bitmap_set_data(bm_, bytes.data(), bytes.size());
}

void BaseBitmap::m_set_matrix(t_symbol* s, const AtomListView& lv)
{
    static const args::ArgChecker chk("NROWS:i>0 NCOLS:i>0 DATA:i+");
    if (!chk.check(lv, this)) {
        return chk.usage(this, s);
    }

    auto num_rows = lv.intAt(0, 0);
    auto num_cols = lv.intAt(1, 0);
    auto data = lv.subView(2);

    std::vector<std::uint8_t> bytes;
    bytes.reserve(data.size());
    for (auto& a : data)
        bytes.push_back(a.asInt());

    ceammc_bitmap_set_matrix(bm_, num_rows, num_cols, 0, 0, bytes.data(), bytes.size());
}

void BaseBitmap::m_set_matrix_at(t_symbol* s, const AtomListView& lv)
{
    static const args::ArgChecker chk("NROWS:i>0 NCOLS:i>0 AT_ROW:i>=0 AT_COL:i>=0 DATA:i+");
    if (!chk.check(lv, this)) {
        return chk.usage(this, s);
    }

    auto num_rows = lv.intAt(0, 0);
    auto num_cols = lv.intAt(1, 0);
    auto at_row = lv.intAt(2, 0);
    auto at_col = lv.intAt(3, 0);

    auto data = lv.subView(4);

    std::vector<std::uint8_t> bytes;
    bytes.reserve(data.size());
    for (auto& a : data)
        bytes.push_back(a.asInt());

    ceammc_bitmap_set_matrix(bm_, num_rows, num_cols, at_row, at_col, bytes.data(), bytes.size());
}

void BaseBitmap::m_hshift(t_symbol* s, const AtomListView& lv)
{
    static const args::ArgChecker chk("DX:i");
    if (!chk.check(lv, this)) {
        return chk.usage(this, s);
    }

    ceammc_bitmap_hshift(bm_, lv.intAt(0, 0));
}

void BaseBitmap::m_font(t_symbol* s, const AtomListView& lv)
{
    static const args::ArgChecker chk("FONT:s");
    if (!chk.check(lv, this)) {
        return chk.usage(this, s);
    }

    ceammc_bitmap_font(bm_, lv.symbolAt(0, &s_)->s_name);
}

void BaseBitmap::m_fill(t_symbol* s, const AtomListView& lv)
{
    ceammc_bitmap_fill(bm_, lv.boolAt(0, false));
}

void BaseBitmap::m_fill_color(t_symbol* s, const AtomListView& lv)
{
    static const args::ArgChecker chk("COLOR:i?");
    if (!chk.check(lv, this)) {
        return chk.usage(this, s);
    }

    ceammc_bitmap_set_fill_color(bm_, lv.intAt(0, -1));
}

void BaseBitmap::m_invert(t_symbol* s, const AtomListView& lv)
{
    ceammc_bitmap_invert(bm_);
}

void BaseBitmap::m_line(t_symbol* s, const AtomListView& lv)
{
    static const args::ArgChecker chk("X0:i Y0:i X1:i Y1:i");
    if (!chk.check(lv, this)) {
        return chk.usage(this, s);
    }

    ceammc_bitmap_draw_line(bm_, lv.intAt(0, 0), lv.intAt(1, 0), lv.intAt(2, 0), lv.intAt(3, 0));
}

void BaseBitmap::m_rect(t_symbol* s, const AtomListView& lv)
{
    static const args::ArgChecker chk("X:i Y:i WIDTH:i>0 HEIGHT:i>0 CENTER:B?");
    if (!chk.check(lv, this)) {
        return chk.usage(this, s);
    }

    auto x = lv.intAt(0, 0);
    auto y = lv.intAt(1, 0);
    auto w = lv.intAt(2, 0);
    auto h = lv.intAt(3, 0);
    auto center = lv.boolAt(4, false);

    ceammc_bitmap_draw_rect(bm_, x, y, w, h, center);
}

void BaseBitmap::m_sector(t_symbol* s, const AtomListView& lv)
{
    static const args::ArgChecker chk("X:i>=0 Y:i>=0 DIAM:i>0 ANGLE:f LEN:f CENTER:B?");
    if (!chk.check(lv, this)) {
        return chk.usage(this, s);
    }

    auto x = lv.intAt(0, 0);
    auto y = lv.intAt(1, 0);
    auto diam = lv.intAt(2, 2);
    auto start = lv.floatAt(3, 2);
    auto length = lv.floatAt(4, 2);
    auto center = lv.boolAt(5, false);

    ceammc_bitmap_draw_sector(bm_, x, y, diam, start, length, center);
}

void BaseBitmap::m_stroke_color(t_symbol* s, const AtomListView& lv)
{
    static const args::ArgChecker chk("COLOR:i?");
    if (!chk.check(lv, this)) {
        return chk.usage(this, s);
    }

    ceammc_bitmap_set_stroke_color(bm_, lv.intAt(0, -1));
}

void BaseBitmap::m_stroke_width(t_symbol* s, const AtomListView& lv)
{
    static const args::ArgChecker chk("WD:i>=0?");
    if (!chk.check(lv, this)) {
        return chk.usage(this, s);
    }

    ceammc_bitmap_set_stroke_width(bm_, lv.intAt(0, 0));
}

void setup_base_bitmap()
{
    ObjectFactory<BaseBitmap> obj("bitmap");

    obj.addMethod("arc", &BaseBitmap::m_arc);
    obj.addMethod("circle", &BaseBitmap::m_circle);
    obj.addMethod("clear", &BaseBitmap::m_clear);
    obj.addMethod("column", &BaseBitmap::m_column);
    obj.addMethod("ellipse", &BaseBitmap::m_ellipse);
    obj.addMethod("fill", &BaseBitmap::m_fill);
    obj.addMethod("fill_color", &BaseBitmap::m_fill_color);
    obj.addMethod("font", &BaseBitmap::m_font);
    obj.addMethod("hshift", &BaseBitmap::m_hshift);
    obj.addMethod("invert", &BaseBitmap::m_invert);
    obj.addMethod("line", &BaseBitmap::m_line);
    obj.addMethod("pixel", &BaseBitmap::m_pixel);
    obj.addMethod("rect", &BaseBitmap::m_rect);
    obj.addMethod("row", &BaseBitmap::m_row);
    obj.addMethod("sector", &BaseBitmap::m_sector);
    obj.addMethod("stroke_color", &BaseBitmap::m_stroke_color);
    obj.addMethod("stroke_width", &BaseBitmap::m_stroke_width);
    obj.addMethod("text", &BaseBitmap::m_text);
    obj.addMethod("triangle", &BaseBitmap::m_triangle);
    obj.addMethod("vshift", &BaseBitmap::m_vshift);

    obj.addMethod("get", &BaseBitmap::m_get);
    obj.addMethod("get_matrix", &BaseBitmap::m_get_matrix);
    obj.addMethod("get_submatrix", &BaseBitmap::m_get_submatrix);

    obj.addMethod("set", &BaseBitmap::m_set);
    obj.addMethod("set_matrix", &BaseBitmap::m_set_matrix);
    obj.addMethod("matrix", &BaseBitmap::m_set_matrix); // alias
    obj.addMethod("set_matrix_at", &BaseBitmap::m_set_matrix_at);
}

/*****************************************************************************
 * Copyright 2018 Serge Poltavsky. All rights reserved.
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
#include "ui_colorpanel.h"
#include "ceammc_convert.h"
#include "ceammc_preset.h"
#include "ceammc_ui.h"

static const int DEFAULT_MATRIX_X = 24;
static const int DEFAULT_MATRIX_Y = 13;

static t_symbol* SYM_PROP_PD;
static t_symbol* SYM_PROP_RGB;
static t_symbol* SYM_PROP_HSL;
static t_symbol* SYM_PROP_HEX;

static int rgb_to_pd(const t_rgba& c)
{
    return -256 * 256 * int(roundf(c.red * 255.f))
        - 256 * int(roundf(c.green * 255.f))
        - 1 * int(roundf(c.blue * 255.f)) - 1;
}

UIColorPanel::UIColorPanel()
    : matrix_x_(DEFAULT_MATRIX_X)
    , matrix_y_(DEFAULT_MATRIX_Y)
    , colors_(matrix_x_, RgbCol(matrix_y_, t_rgba()))
    , hover_layer_(asEBox(), gensym("hover_layer"))
    , picked_layer_(asEBox(), gensym("picked_layer"))
    , saturation_(1)
    , hue_(0)
    , lightness_(1)
    , prop_reverse_(0)
    , picked_x_(-1)
    , picked_y_(-1)
    , hover_x_(-1)
    , hover_y_(-1)
{
    createOutlet();
}

void UIColorPanel::okSize(t_rect* newrect)
{
    newrect->width = pd_clip_min(newrect->width, 49);
    newrect->height = pd_clip_min(newrect->height, 10.);

    float ratio = (newrect->width - 1) / matrix_x_;

    if (ratio - (int)ratio != 0) {
        ratio = roundf(ratio);
        newrect->width = ratio * matrix_x_ + 1;
    }

    ratio = (newrect->height - 1) / matrix_y_;
    if (ratio - (int)ratio != 0) {
        ratio = roundf(ratio);
        newrect->height = ratio * matrix_y_ + 1;
    }

    newrect->width = pd_clip_min(newrect->width, 49);
    newrect->height = pd_clip_min(newrect->height, 10);
}

void UIColorPanel::onPropChange(t_symbol* prop_name)
{
    computeColors();
}

void UIColorPanel::paint()
{
    drawBackground();
    drawHover();
    drawPicked();
}

void UIColorPanel::drawBackground()
{
    const auto r = rect();

    UIPainter p = bg_layer_.painter(r);
    if (!p)
        return;

    int block_width = r.width / matrix_x_;
    int block_height = r.height / matrix_y_;

    for (int incx = 0, i = 0; i < matrix_x_; i++, incx += block_width) {
        for (int incY = 0, j = 0; j < matrix_y_; j++, incY += block_height) {
            p.setColor(colors_[i][j]);
            p.drawRect(incx + 1, incY + 1,
                clip<int>(block_width - 1, 1, 100),
                clip<int>(block_height - 1, 1, 100));
            p.fill();
        }
    }
}

void UIColorPanel::drawHover()
{
    const auto r = rect();

    UIPainter p = hover_layer_.painter(r);
    if (!p)
        return;

    if (hover_x_ >= 0 && hover_y_ >= 0) {
        int cell_w = r.width / matrix_x_;
        int cell_h = r.height / matrix_y_;

        p.setColor(colors_[hover_x_][hover_y_]);
        p.drawRect(hover_x_ * cell_w - 3, hover_y_ * cell_h - 3, cell_w + 6, cell_h + 6);
        p.fillPreserve();
        p.setColor(rgba_white);
        p.stroke();
    }
}

void UIColorPanel::drawPicked()
{
    const auto r = rect();

    UIPainter p = picked_layer_.painter(r);
    if (!p)
        return;

    if (picked_x_ >= 0 && picked_y_ >= 0) {
        int cell_w = r.width / matrix_x_;
        int cell_h = r.height / matrix_y_;
        int rw = std::max<int>(cell_w - 2, 1);
        int rh = std::max<int>(cell_h - 2, 1);

        p.setColor(prop_color_border);
        p.setLineWidth(2);
        p.drawRect(picked_x_ * cell_w + 1, picked_y_ * cell_h + 1, rw, rh);
        p.stroke();
    }
}

void UIColorPanel::onMouseDown(t_object* view, const t_pt& pt, const t_pt& abs_pt, long modifiers)
{
    const auto r = rect();

    hover_x_ = -1;
    hover_y_ = -1;

    picked_x_ = clip<int>(pt.x / (r.width / matrix_x_), 0, matrix_x_ - 1);
    picked_y_ = clip<int>(pt.y / (r.height / matrix_y_), 0, matrix_y_ - 1);

    hover_layer_.invalidate();
    picked_layer_.invalidate();
    redraw();

    output();
}

void UIColorPanel::onMouseLeave(t_object* view, const t_pt& pt, long modifiers)
{
    hover_x_ = -1;
    hover_y_ = -1;
    hover_layer_.invalidate();
    redraw();
}

void UIColorPanel::onMouseMove(t_object* view, const t_pt& pt, long modifiers)
{
    const auto r = rect();
    hover_x_ = clip<int>(pt.x / (r.width / matrix_x_), 0, matrix_x_ - 1);
    hover_y_ = clip<int>(pt.y / (r.height / matrix_y_), 0, matrix_y_ - 1);
    hover_layer_.invalidate();
    redraw();
}

void UIColorPanel::onBang()
{
    output();
}

AtomList UIColorPanel::propMatrixSize() const
{
    return AtomList(Atom(matrix_x_), Atom(matrix_y_));
}

void UIColorPanel::propSetMatrixSize(const AtomList& lst)
{
    bool ok = lst.size() > 1 && lst[0].isFloat() && lst[1].isFloat();

    if (!ok) {
        UI_ERR << "matrix size expected: X Y";
        return;
    }

    matrix_x_ = clip<int>(lst[0].asFloat(), 1, 32);
    matrix_y_ = clip<int>(lst[1].asFloat(), 1, 32);
    colors_.assign(matrix_x_, RgbCol(matrix_y_, t_rgba()));

    t_rect r = rect();
    resize(r.width, r.height);
    computeColors();
}

AtomList UIColorPanel::propColorIndex() const
{
    return AtomList(picked_x_, picked_y_);
}

t_float UIColorPanel::propPdColor() const
{
    if (picked_x_ < 0 || picked_y_ < 0)
        return 0;

    return rgb_to_pd(colors_[picked_x_][picked_y_]);
}

AtomList UIColorPanel::propRgbColor() const
{
    AtomList res;
    res.fill(Atom(1), 3);

    if (picked_x_ < 0 || picked_y_ < 0)
        return res;

    const t_rgba& c = colors_[picked_x_][picked_y_];
    res[0].setFloat(c.red);
    res[1].setFloat(c.green);
    res[2].setFloat(c.blue);
    return res;
}

AtomList UIColorPanel::propHslColor() const
{
    AtomList res;
    res.fill(Atom(1), 3);

    if (picked_x_ < 0 || picked_y_ < 0)
        return res;

    t_hsla c = rgba_to_hsla(colors_[picked_x_][picked_y_]);
    res[0].setFloat(c.hue);
    res[1].setFloat(c.saturation);
    res[2].setFloat(c.lightness);
    return res;
}

AtomList UIColorPanel::propHexColor() const
{
    if (picked_x_ < 0 || picked_y_ < 0)
        return AtomList(gensym("#FFFFFF"));

    return Atom(gensym(rgba_to_hex(colors_[picked_x_][picked_y_])));
}

void UIColorPanel::m_set(const AtomList& lst)
{
    if (lst.size() < 2) {
        UI_ERR << "color indexes expected: X Y";
        return;
    }

    t_float x, y;
    if (!lst[0].getFloat(&x) || !lst[1].getFloat(&y)) {
        UI_ERR << "color indexes excpected: X Y";
        return;
    }

    if (!(0 <= x && x < matrix_x_) || !(0 <= y && y < matrix_y_)) {
        UI_ERR << "invalid indexes: " << lst;
        return;
    }

    picked_x_ = x;
    picked_y_ = y;

    picked_layer_.invalidate();
    redraw();
}

void UIColorPanel::loadPreset(size_t idx)
{
    AtomList lst = PresetStorage::instance().listValueAt(presetId(), idx);
    m_set(lst);
    output();
}

void UIColorPanel::storePreset(size_t idx)
{
    PresetStorage::instance().setListValueAt(presetId(), idx, AtomList(Atom(picked_x_), Atom(picked_y_)));
}

template <class T>
void wrapRef(T& v, T min, T max)
{
    if (v > max)
        v = v - (max - min);
    else if (v < min)
        v = v + (max - min);
}

/* wrap x -> [0,max) */
static float wrapMax(float x, float max)
{
    /* integer math: `(max + x % max) % max` */
    return fmodf(max + fmodf(x, max), max);
}

/* wrap x -> [min,max) */
static float wrapMinMax(float x, float min, float max)
{
    return min + wrapMax(x - min, max - min);
}

void UIColorPanel::computeColors()
{
    t_pt f_color_picked;
    hover_x_ = -1;
    hover_y_ = -1;

    f_color_picked.x = -10;
    f_color_picked.y = -10;

    t_hsla color_ref;
    hsla_set(&color_ref, hue_, saturation_, lightness_, 1.);

    const float X_STEP = 1.f / matrix_x_;
    const float Y_STEP = 1.f / matrix_y_;

    if (prop_reverse_) {
        for (int j = 0; j < matrix_y_; j++) {
            color_ref.lightness = lightness_;

            for (int i = 0; i < matrix_x_; i++) {
                colors_[i][j] = hsla_to_rgba(color_ref);
                color_ref.lightness = wrapMax(color_ref.lightness - X_STEP, 1);
            }

            color_ref.hue = wrapMax(color_ref.hue + Y_STEP, 1);
        }
    } else {
        for (int i = 0; i < matrix_x_; i++) {
            color_ref.lightness = lightness_;

            for (int j = 0; j < matrix_y_; j++) {
                colors_[i][j] = hsla_to_rgba(color_ref);
                color_ref.lightness = wrapMax(color_ref.lightness - Y_STEP, 1);
            }

            color_ref.hue = wrapMax(color_ref.hue + X_STEP, 1);
        }
    }

    redrawAll();
}

void UIColorPanel::redrawAll()
{
    hover_layer_.invalidate();
    picked_layer_.invalidate();
    bg_layer_.invalidate();
    redraw();
}

void UIColorPanel::output()
{
    if (picked_x_ < 0 || picked_y_ < 0)
        return;

    const t_rgba& color_rgb = colors_[picked_x_][picked_y_];
    t_hsla color_hls = rgba_to_hsla(color_rgb);
    t_symbol* color_hex = gensym(rgba_to_hex(color_rgb));

    AtomList out;
    out.reserve(4);

    // output pd
    out.append(Atom(rgb_to_pd(color_rgb)));
    anyTo(0, SYM_PROP_PD, out);
    send(SYM_PROP_PD, out);

    // output rgb
    out.fill(Atom(0.f), 3);
    out[0].setFloat(color_rgb.red);
    out[1].setFloat(color_rgb.green);
    out[2].setFloat(color_rgb.blue);
    anyTo(0, SYM_PROP_RGB, out);
    send(SYM_PROP_RGB, out);

    // hsl
    out[0].setFloat(color_hls.hue);
    out[1].setFloat(color_hls.saturation);
    out[2].setFloat(color_hls.lightness);
    anyTo(0, SYM_PROP_HSL, out);
    send(SYM_PROP_HSL, out);

    // hex
    out.resizeClip(1);
    out[0].setSymbol(color_hex, true);
    anyTo(0, SYM_PROP_HEX, out);
    send(SYM_PROP_HEX, out);
}

void UIColorPanel::setup()
{
    UIObjectFactory<UIColorPanel> obj("ui.colorpanel");

    obj.setDefaultSize(181, 105);
    obj.useMouseEvents(UI_MOUSE_DOWN | UI_MOUSE_LEAVE | UI_MOUSE_MOVE);
    obj.usePresets();
    obj.useBang();

    obj.addProperty("matrix", &UIColorPanel::propMatrixSize, &UIColorPanel::propSetMatrixSize);
    obj.showProperty("matrix");
    obj.setPropertyCategory("matrix", "Basic");
    obj.setPropertyLabel("matrix", _("Matrix Size"));

    obj.addFloatProperty("saturation", _("Saturation"), 1., &UIColorPanel::saturation_);
    obj.setPropertyRange("saturation", 0, 1);
    obj.setPropertyStep("saturation", 0.1);

    obj.addFloatProperty("hue", _("First Hue"), 0., &UIColorPanel::hue_);
    obj.setPropertyRange("hue", 0, 1);
    obj.setPropertyStep("hue", 0.1);

    obj.addFloatProperty("lightness", _("First Lightness"), 1., &UIColorPanel::lightness_);
    obj.setPropertyRange("lightness", 0, 1);
    obj.setPropertyStep("lightness", 0.1);

    obj.addProperty("reverse", _("Matrix Reversed"), false, &UIColorPanel::prop_reverse_);

    obj.addProperty("index", &UIColorPanel::propColorIndex, &UIColorPanel::m_set);
    obj.addMethod("set", &UIColorPanel::m_set);

    obj.addProperty("pd", &UIColorPanel::propPdColor, 0);
    obj.addProperty("rgb", &UIColorPanel::propRgbColor, 0);
    obj.addProperty("hsl", &UIColorPanel::propHslColor, 0);
    obj.addProperty("hex", &UIColorPanel::propHexColor);
}

void setup_ui_colorpanel()
{
    SYM_PROP_PD = gensym("@pd");
    SYM_PROP_RGB = gensym("@rgb");
    SYM_PROP_HSL = gensym("@hsl");
    SYM_PROP_HEX = gensym("@hex");

    UIColorPanel::setup();
}

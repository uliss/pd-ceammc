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

#include <algorithm>
#include <stdio.h>

#include "ceammc_atomlist.h"
#include "ceammc_convert.h"
#include "ceammc_format.h"

#include "ceammc_preset.h"
#include "ceammc_ui.h"
#include "ui_slider2d.h"
#include "ui_slider2d.tcl.h"

UISlider2D::UISlider2D()
    : prop_x_left(-1)
    , prop_x_right(1)
    , prop_y_top(1)
    , prop_y_bottom(-1)
    , prop_show_range(1)
    , prop_show_grid(0)
    , x_pos_(convert::lin2lin<float, 0, 1>(0.5, prop_x_left, prop_x_right))
    , y_pos_(convert::lin2lin<float, 0, 1>(0.5, prop_y_top, prop_y_bottom))
    , mouse_down_(false)
{
    createOutlet();

    auto xmid = [this] { return convert::lin2lin<t_float, 0, 1>(0.5, prop_x_left, prop_x_right); };
    auto ymid = [this] { return convert::lin2lin<t_float, 0, 1>(0.5, prop_y_top, prop_y_bottom); };

    initPopupMenu("slider2d",
        { { _("center"), [this, xmid, ymid](const t_pt&) {
               onList(AtomList(xmid(), ymid()));
           } },
            { _("left center"), [this, ymid](const t_pt&) {
                 onList(AtomList(prop_x_left, ymid()));
             } },
            { _("right center"), [this, ymid](const t_pt&) {
                 onList(AtomList(prop_x_right, ymid()));
             } },
            { _("top center"), [this, xmid](const t_pt&) {
                 onList(AtomList(xmid(), prop_y_top));
             } },
            { _("bottom center"), [this, xmid](const t_pt&) {
                 onList(AtomList(xmid(), prop_y_bottom));
             } } });
}

void UISlider2D::okSize(t_rect* newrect)
{
    newrect->height = std::max<float>(30, newrect->height);
    newrect->width = std::max<float>(30, newrect->width);
}

void UISlider2D::paint()
{
    const t_rect r = rect();

    const int x = std::round(convert::lin2lin_clip<float>(x_pos_, prop_x_left, prop_x_right, 0, r.width));
    const int y = std::round(convert::lin2lin<float>(y_pos_, prop_y_top, prop_y_bottom, 0, r.height));

    sys_vgui("ui::slider2d_update %s %lx "
             "%d %d " // x, y
             "%.5g %.5g " // value x, y
             "%d %d %d " // w, h, z
             "%d %d %d "
             "#%6.6x #%6.6x "
             "%.3g %.3g %.3g %.3g\n",
        asEBox()->b_canvas_id->s_name, asEBox(),
        x, y,
        realXValue(), realYValue(),
        (int)width(), (int)height(), (int)zoom(),
        prop_show_grid, prop_show_range, mouse_down_ ? 1 : 0,
        rgba_to_hex_int(prop_color_background), rgba_to_hex_int(prop_color_border),
        prop_x_left, prop_x_right, prop_y_top, prop_y_bottom);
}

void UISlider2D::onPropChange(t_symbol* prop_name)
{
    redrawAll();
}

void UISlider2D::onBang()
{
    output();
}

void UISlider2D::onList(const AtomListView& lv)
{
    if (lv.size() != 2) {
        UI_ERR << "invalid list given: X Y expected";
        return;
    }

    if (!setRealValue(lv))
        return;

    redrawKnob();
    output();
}

void UISlider2D::onMouseDown(t_object* view, const t_pt& pt, const t_pt& abs_pt, long modifiers)
{
    mouse_down_ = true;
    setMouse(pt.x, pt.y);
    redrawKnob();
    output();
}

void UISlider2D::onMouseDrag(t_object* view, const t_pt& pt, long modifiers)
{
    setMouse(pt.x, pt.y);
    redrawKnob();
    output();
}

void UISlider2D::onMouseUp(t_object* view, const t_pt& pt, long modifiers)
{
    mouse_down_ = false;
    setMouse(pt.x, pt.y);
    redrawKnob();
    output();
}

void UISlider2D::m_set(const AtomListView& lv)
{
    if (lv.size() != 2) {
        UI_ERR << "invalid list given: set X Y expected";
        return;
    }

    if (!setRealValue(lv))
        return;

    redrawKnob();
}

void UISlider2D::m_move(const AtomListView& lv)
{
    if (lv.size() != 2) {
        UI_ERR << "invalid data: X Y offset expected: " << lv;
        return;
    }

    t_float x, y;
    if (!lv[0].getFloat(&x) || !lv[1].getFloat(&y)) {
        UI_ERR << "float offsets are expected: " << lv;
        return;
    }

    if (!setRealValue(AtomList(realXValue() + x, realYValue() + y)))
        return;

    redrawKnob();
}

void UISlider2D::loadPreset(size_t idx)
{
    Atom def[2] = { 0., 0. };

    auto lv = PresetStorage::instance().listValueAt(presetId(), idx, AtomListView(def, 2));
    setRealValue(lv);
    redrawKnob();
    output();
}

void UISlider2D::storePreset(size_t idx)
{
    PresetStorage::instance().setListValueAt(presetId(), idx, realValue());
}

static AtomList interp_lists(const AtomListView& lv0, const AtomListView& lv1, size_t n, float k)
{
    Atom res[n];

    for (size_t i = 0; i < n; i++) {
        auto v0 = lv0.floatAt(i, 0);
        auto v1 = lv1.floatAt(i, 0);
        res[i] = v0 * (1 - k) + v1 * k;
    }

    return AtomList(AtomListView(res, n));
}

void UISlider2D::interpPreset(t_float idx)
{
    Atom def[2] = { 0.f, 0.f };
    auto lv0 = PresetStorage::instance().listValueAt(presetId(), static_cast<int>(idx), AtomListView(def, 2));
    auto lv1 = PresetStorage::instance().listValueAt(presetId(), static_cast<int>(idx) + 1, AtomListView(def, 2));

    float k = (static_cast<float>(idx) - static_cast<int>(idx));
    onList(interp_lists(lv0, lv1, 2, k));
}

bool UISlider2D::setRealValue(const AtomListView& lv)
{
    if (lv.size() != 2)
        return false;

    t_float x, y;
    if (!lv[0].getFloat(&x) || !lv[1].getFloat(&y)) {
        UI_ERR << "invalid value: " << lv;
        return false;
    }

    if (xRange() == 0 || yRange() == 0) {
        UI_ERR << "invalid slider range: " << xRange() << " - " << yRange();
        return false;
    }

    x_pos_ = clip_any<t_float>(x, prop_x_left, prop_x_right);
    y_pos_ = clip_any<t_float>(y, prop_y_top, prop_y_bottom);
    return true;
}

AtomList UISlider2D::realValue() const
{
    return AtomList(realXValue(), realYValue());
}

t_float UISlider2D::realXValue() const
{
    return clip_any<float>(x_pos_, prop_x_left, prop_x_right);
}

t_float UISlider2D::realYValue() const
{
    return clip_any<float>(y_pos_, prop_y_top, prop_y_bottom);
}

void UISlider2D::output()
{
    Atom res[2];
    res[0] = realXValue();
    res[1] = realYValue();

    AtomListView lv(res, 2);
    listTo(0, lv);
    send(lv);
}

t_float UISlider2D::propXRange() const
{
    return xRange();
}

t_float UISlider2D::propYRange() const
{
    return yRange();
}

t_float UISlider2D::propXValue() const
{
    return realXValue();
}

t_float UISlider2D::propYValue() const
{
    return realYValue();
}

void UISlider2D::propSetXValue(t_float x)
{
    if (xRange() == 0) {
        UI_ERR << "invalid x-range: " << xRange();
        return;
    }

    x_pos_ = clip_any<t_float>(x, prop_x_left, prop_x_right);

    redrawKnob();
}

void UISlider2D::propSetYValue(t_float y)
{
    if (yRange() == 0) {
        UI_ERR << "invalid y-range: " << yRange();
        return;
    }

    y_pos_ = clip_any<t_float>(y, prop_y_top, prop_y_bottom);

    redrawKnob();
}

void UISlider2D::setup()
{
    UIObjectFactory<UISlider2D> obj("ui.slider2d");
    obj.addAlias("ui.s2d");

    obj.setDefaultSize(100, 100);

    obj.addFloatProperty("x_left", _("Left X-value"), -1, &UISlider2D::prop_x_left, "Bounds");
    obj.addFloatProperty("x_right", _("Right X-value"), 1, &UISlider2D::prop_x_right, "Bounds");
    obj.addFloatProperty("y_top", _("Top Y-value"), 1, &UISlider2D::prop_y_top, "Bounds");
    obj.addFloatProperty("y_bottom", _("Bottom Y-value"), -1, &UISlider2D::prop_y_bottom, "Bounds");

    obj.addProperty("x_range", &UISlider2D::propXRange);
    obj.setPropertyDefaultValue("x_range", "2");
    obj.addProperty("y_range", &UISlider2D::propYRange);
    obj.setPropertyDefaultValue("y_range", "2");
    obj.addProperty("x_value", &UISlider2D::propXValue, &UISlider2D::propSetXValue);
    obj.addProperty("y_value", &UISlider2D::propYValue, &UISlider2D::propSetYValue);

    obj.addProperty("show_grid", _("Show grid"), false, &UISlider2D::prop_show_grid, "Main");
    obj.addProperty("show_range", _("Show range"), true, &UISlider2D::prop_show_range, "Main");

    obj.usePresets();
    obj.useList();
    obj.useBang();

    obj.useMouseEvents(UI_MOUSE_UP | UI_MOUSE_DOWN | UI_MOUSE_DRAG);
    obj.outputMouseEvents(MouseEventsOutput::DEFAULT_OFF);
    obj.usePopup();

    obj.addMethod("set", &UISlider2D::m_set);
    obj.addMethod("move", &UISlider2D::m_move);
}

void UISlider2D::redrawKnob()
{
    redraw();
}

void UISlider2D::redrawAll()
{
    bg_layer_.invalidate();
    redraw();
}

void UISlider2D::setMouse(t_float x, t_float y)
{
    x_pos_ = convert::lin2lin_clip<t_float>(x, 0, width(), prop_x_left, prop_x_right);
    y_pos_ = convert::lin2lin_clip<t_float>(y, 0, height(), prop_y_top, prop_y_bottom);
}

void setup_ui_slider2d()
{
    sys_gui(ui_slider2d_tcl);

    UISlider2D::setup();
}

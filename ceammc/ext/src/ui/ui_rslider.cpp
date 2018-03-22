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
#include "ui_rslider.h"
#include "ceammc_convert.h"
#include "ceammc_preset.h"
#include "ceammc_ui.h"

#include <boost/algorithm/minmax.hpp>

UIRSlider::UIRSlider()
    : prop_color_knob(rgba_blue)
    , prop_min(0)
    , prop_max(1)
    , prop_mouse_sync(0)
    , knob_layer_(asEBox(), gensym("knob_layer"))
    , vlow_(0)
    , vhigh_(0)
    , is_horizontal_(true)
    , drag_mode_(NONE)
{
    createOutlet();
}

void UIRSlider::init(t_symbol* name, const AtomList& args, bool usePresets)
{
    UIObject::init(name, args, usePresets);
    vlow_ = convert::lin2lin<t_float>(0.4, 0, 1, prop_min, prop_max);
    vhigh_ = convert::lin2lin<t_float>(0.6, 0, 1, prop_min, prop_max);
}

t_pd_err UIRSlider::notify(t_symbol* attr_name, t_symbol* msg)
{
    if (msg == s_attr_modified) {
        redrawBGLayer();
        redrawKnob();
    }

    return 0;
}

void UIRSlider::okSize(t_rect* newrect)
{
    newrect->width = pd_clip_min(newrect->width, 16);
    newrect->height = pd_clip_min(newrect->height, 16);

    is_horizontal_ = (newrect->width > newrect->height);

    if (is_horizontal_) {
        newrect->width = pd_clip_min(newrect->width, 50);
        if ((int)newrect->height % 2 == 1)
            newrect->height++;
    } else {
        newrect->height = pd_clip_min(newrect->height, 50);
        if ((int)newrect->width % 2 == 1)
            newrect->width++;
    }
}

void UIRSlider::paint(t_object* view)
{
    drawBackground();
    drawKnob();
}

void UIRSlider::drawBackground()
{
    const t_rect& r = rect();
    UIPainter p = bg_layer_.painter(r);
    if (!p)
        return;

    p.setColor(prop_color_border);
    p.setLineWidth(2);

    if (is_horizontal_)
        p.drawLine(-1, r.height * 0.5, r.width + 2, r.height * 0.5);
    else
        p.drawLine(r.width * 0.5, -2, r.width * 0.5, r.height + 4);
}

void UIRSlider::drawKnob()
{
    const t_rect& r = rect();
    UIPainter p = knob_layer_.painter(r);
    if (!p)
        return;

    if (prop_max == prop_min)
        return;

    t_rgba rect_color = rgba_color_sum(&prop_color_background, &prop_color_knob, 0.5);
    t_rgba line_color = rgba_color_sum(&prop_color_border, &prop_color_knob, 0.5);

    float value_low = convert::lin2lin_clip<float>(vlow_, prop_min, prop_max, 0, 1);
    float value_high = convert::lin2lin_clip<float>(vhigh_, prop_min, prop_max, 0, 1);

    p.setLineWidth(2);
    if (is_horizontal_) {
        float x0 = value_low * r.width;
        float x1 = value_high * r.width;

        p.setColor(rect_color);
        p.drawRect(x0, -2, x1 - x0, r.height + 4);
        p.fill();

        p.setColor(line_color);
        p.drawLine(x0, r.height * 0.5, x1, r.height * 0.5);

        p.setColor(prop_color_knob);
        p.drawLine(x0, -2, x0, r.height + 4);
        p.drawLine(x1, -2, x1, r.height + 4);
    } else {
        p.setColor(rect_color);
        p.drawRect(-2, value_low * r.height, r.width + 4, value_high * r.height - value_low * r.height);
        p.fill();

        p.setColor(line_color);
        p.drawLine(r.width * 0.5, value_low * r.height, r.width * 0.5, value_high * r.height);

        p.setColor(prop_color_knob);
        p.drawLine(-2, value_low * r.height, r.width + 4, value_low * r.height);
        p.drawLine(-2, value_high * r.height, r.width + 4, value_high * r.height);
    }
}

void UIRSlider::onBang()
{
    output();
}

void UIRSlider::onList(const AtomList& lst)
{
    if (!setValue(lst))
        return;

    redrawKnob();
    output();
}

void UIRSlider::onMouseDown(t_object* view, const t_pt& pt, long modifiers)
{
    const t_rect& r = rect();

    float value = 0;

    if (is_horizontal_)
        value = convert::lin2lin_clip<float>(pt.x, 0, r.width, prop_min, prop_max);
    else
        value = convert::lin2lin_clip<float>(pt.y, 0, r.height, prop_min, prop_max);

    if (modifiers == EMOD_SHIFT) {
        if (fabs(vhigh_ - value) < fabs(vlow_ - value)) {
            vhigh_ = value;
            drag_mode_ = HIGH;
        } else {
            vlow_ = value;
            drag_mode_ = LOW;
        }
    } else {
        vlow_ = vhigh_ = value;
        drag_mode_ = NONE;
        click_pt_ = pt;
    }

    adjustValues();
    redrawKnob();
    if (prop_mouse_sync)
        output();
}

void UIRSlider::onMouseUp(t_object* view, const t_pt& pt, long modifiers)
{
    if (!prop_mouse_sync)
        output();
}

void UIRSlider::onMouseDrag(t_object* view, const t_pt& pt, long modifiers)
{
    const t_rect& r = rect();

    float value;

    if (is_horizontal_)
        value = convert::lin2lin_clip<float>(pt.x, 0, r.width, prop_min, prop_max);
    else
        value = convert::lin2lin_clip<float>(pt.y, 0, r.height, prop_min, prop_max);

    if (drag_mode_ == LOW)
        vlow_ = value;
    else if (drag_mode_ == HIGH)
        vhigh_ = value;
    else {
        bool backward = false;

        // right -> left selection
        if (is_horizontal_ && pt.x < click_pt_.x)
            backward = true;
        // top -> bottom selection
        else if (!is_horizontal_ && pt.y < click_pt_.y)
            backward = true;

        if (backward)
            vlow_ = value;
        else
            vhigh_ = value;
    }

    adjustValues();
    redrawKnob();
    if (prop_mouse_sync)
        output();
}

AtomList UIRSlider::propValue() const
{
    return AtomList(vlow_, vhigh_);
}

void UIRSlider::propSetValue(const AtomList& lst)
{
    if (!setValue(lst))
        return;

    redrawKnob();
}

t_float UIRSlider::propLow() const
{
    return vlow_;
}

t_float UIRSlider::propHigh() const
{
    return vhigh_;
}

void UIRSlider::propSetLow(t_float f)
{
    vlow_ = clip_any<float>(f, prop_min, prop_max);
    adjustValues();
    redrawKnob();
}

void UIRSlider::propSetHigh(t_float f)
{
    vhigh_ = clip_any<float>(f, prop_min, prop_max);
    adjustValues();
    redrawKnob();
}

t_float UIRSlider::propRange() const
{
    return vhigh_ - vlow_;
}

void UIRSlider::loadPreset(size_t idx)
{
    onList(PresetStorage::instance().listValueAt(presetId(), idx));
}

void UIRSlider::storePreset(size_t idx)
{
    PresetStorage::instance().setListValueAt(presetId(), idx, propValue());
}

void UIRSlider::adjustValues()
{
    boost::tuple<float, float> v = boost::minmax(vlow_, vhigh_);

    if (prop_min < prop_max) {
        vlow_ = v.get<0>();
        vhigh_ = v.get<1>();
    } else {
        vlow_ = v.get<1>();
        vhigh_ = v.get<0>();
    }
}

void UIRSlider::redrawKnob()
{
    knob_layer_.invalidate();
    redraw();
}

void UIRSlider::output()
{
    AtomList res(vlow_, vhigh_);
    listTo(0, res);
    send(res);
}

bool UIRSlider::setValue(const AtomList& lst)
{
    if (lst.size() != 2) {
        UI_ERR << "min max values expected: " << lst;
        return false;
    }

    const size_t N = lst.size();

    boost::tuple<float, float> range = boost::minmax(prop_min, prop_max);

    if (N > 0 && lst[0].isFloat())
        vlow_ = clip<float>(lst[0].asFloat(), range.get<0>(), range.get<1>());

    if (N > 1 && lst[1].isFloat())
        vhigh_ = clip<float>(lst[1].asFloat(), range.get<0>(), range.get<1>());

    adjustValues();

    return true;
}

void UIRSlider::setup()
{
    UIObjectFactory<UIRSlider> obj("ui.rslider");
    obj.addAlias("ui.rsl");
    obj.setDefaultSize(120, 15);

    obj.useMouseEvents(UI_MOUSE_DOWN | UI_MOUSE_UP | UI_MOUSE_DRAG);
    obj.usePresets();
    obj.useBang();
    obj.useList();

    obj.addProperty("min", _("Minimum Value"), 0.f, &UIRSlider::prop_min);
    obj.addProperty("max", _("Maximum Value"), 1.f, &UIRSlider::prop_max);
    obj.addProperty("sync", _("Mouse sync"), false, &UIRSlider::prop_mouse_sync);
    obj.addProperty("knob_color", _("Knob Color"), DEFAULT_ACTIVE_COLOR, &UIRSlider::prop_color_knob);

    obj.addProperty("value", &UIRSlider::propValue, &UIRSlider::propSetValue);
    obj.addProperty("range", &UIRSlider::propRange, 0);
    obj.addProperty("low", &UIRSlider::propLow, &UIRSlider::propSetLow);
    obj.addProperty("high", &UIRSlider::propHigh, &UIRSlider::propSetHigh);

    obj.addMethod("set", &UIRSlider::propSetValue);
}

void setup_ui_rslider()
{
    UIRSlider::setup();
}

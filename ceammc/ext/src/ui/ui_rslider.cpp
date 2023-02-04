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

#include <tuple>

// CREATE_MODE -> MOVE_MOVE -> CHANGE_MODE

UIRSlider::UIRSlider()
    : prop_color_knob(rgba_blue)
    , prop_min(0)
    , prop_max(1)
    , knob_layer_(asEBox(), gensym("knob_layer"))
    , vlow_(0)
    , vhigh_(0)
    , is_horizontal_(true)
    , drag_mode_(NONE)
    , edit_mode_(CREATE)
{
    createOutlet();
}

void UIRSlider::init(t_symbol* name, const AtomListView& args, bool usePresets)
{
    UIObject::init(name, args, usePresets);
    vlow_ = convert::lin2lin<t_float>(0.4, 0, 1, prop_min, prop_max);
    vhigh_ = convert::lin2lin<t_float>(0.6, 0, 1, prop_min, prop_max);
}

void UIRSlider::onPropChange(t_symbol* prop_name)
{
    redrawBGLayer();
    redrawKnob();
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

void UIRSlider::paint()
{
    drawBackground();
    drawKnob();
}

void UIRSlider::drawBackground()
{
    const t_rect r = rect();
    UIPainter p = bg_layer_.painter(r);
    if (!p)
        return;

    p.setColor(prop_color_border);
    p.setLineWidth(2);

    if (is_horizontal_)
        p.drawLine(0, r.height * 0.5, r.width, r.height * 0.5);
    else
        p.drawLine(r.width * 0.5, 0, r.width * 0.5, r.height);
}

void UIRSlider::drawKnob()
{
    const t_rect r = rect();
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
        p.drawRect(x0, 0, x1 - x0, r.height);
        p.fill();

        p.setColor(line_color);
        p.drawLine(x0, r.height * 0.5, x1, r.height * 0.5);

        p.setColor(prop_color_knob);
        p.drawLine(x0, 0, x0, r.height);
        p.drawLine(x1, 0, x1, r.height);
    } else {
        p.setColor(rect_color);
        p.drawRect(0, value_low * r.height, r.width, value_high * r.height - value_low * r.height);
        p.fill();

        p.setColor(line_color);
        p.drawLine(r.width * 0.5, value_low * r.height, r.width * 0.5, value_high * r.height);

        p.setColor(prop_color_knob);
        p.drawLine(0, value_low * r.height, r.width, value_low * r.height);
        p.drawLine(0, value_high * r.height, r.width, value_high * r.height);
    }
}

void UIRSlider::onBang()
{
    output();
}

void UIRSlider::onList(const AtomListView& lv)
{
    if (!setValue(lv))
        return;

    redrawKnob();
    output();
}

UIRSlider::EditMode UIRSlider::keyMod2EditMode(long mod, float value) const
{
    if (mod & EMOD_SHIFT)
        return MOVE;
    else if (mod & EMOD_ALT) {
        if (std::abs(vhigh_ - value) < std::abs(vlow_ - value))
            return CHANGE_HIGH;
        else
            return CHANGE_LOW;
    }
#ifdef __APPLE__
    else if (mod & EMOD_CMD)
        return OUTPUT;
#else
    else if (mod & EMOD_CTRL)
        return OUTPUT;
#endif
    else
        return CREATE;
}

void UIRSlider::onMouseDown(t_object* view, const t_pt& pt, const t_pt& abs_pt, long modifiers)
{
    const t_rect r = rect();

    float value = 0;

    if (is_horizontal_)
        value = convert::lin2lin_clip<float>(pt.x, 0, r.width, prop_min, prop_max);
    else
        value = convert::lin2lin_clip<float>(pt.y, 0, r.height, prop_min, prop_max);

    edit_mode_ = keyMod2EditMode(modifiers, value);
    switch (edit_mode_) {
    case CREATE:
        vlow_ = vhigh_ = value;
        click_pt_ = pt;
        break;
    case MOVE:
        click_pt_ = pt;
        break;
    case CHANGE_HIGH:
        vhigh_ = value;
        break;
    case CHANGE_LOW:
        vlow_ = value;
        break;
    default:
    case OUTPUT:
        break;
    }

    adjustValues();
    redrawKnob();
    output();
}

void UIRSlider::onMouseUp(t_object* view, const t_pt& pt, long modifiers)
{
    output();
    edit_mode_ = CREATE;
}

void UIRSlider::onMouseDrag(t_object* view, const t_pt& pt, long modifiers)
{
    const t_rect r = rect();

    float value;

    if (is_horizontal_)
        value = convert::lin2lin_clip<float>(pt.x, 0, r.width, prop_min, prop_max);
    else
        value = convert::lin2lin_clip<float>(pt.y, 0, r.height, prop_min, prop_max);

    switch (edit_mode_) {
    case CREATE: {
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
    } break;
    case MOVE: {
        const float pix_delta = is_horizontal_ ? (pt.x - click_pt_.x) : (pt.y - click_pt_.y);
        const float val_delta = (pix_delta / r.width) * (prop_max - prop_min);

        const float new_low = vlow_ + val_delta;
        const float new_high = vhigh_ + val_delta;

        auto in_range = [](float v, const std::pair<float, float>& range) {
            return range.first <= v && v <= range.second;
        };

        const auto range = std::minmax(prop_min, prop_max);
        if (in_range(new_low, range)
            && in_range(new_high, range)) {
            vhigh_ = new_high;
            vlow_ = new_low;
        }

        click_pt_ = pt;
    } break;
    case CHANGE_LOW: {
        vlow_ = value;
    } break;
    case CHANGE_HIGH: {
        vhigh_ = value;
    } break;
    default:
        break;
    }

    adjustValues();
    redrawKnob();
    output();
}

AtomList UIRSlider::propValue() const
{
    return AtomList(vlow_, vhigh_);
}

void UIRSlider::propSetValue(const AtomListView& lv)
{
    if (!setValue(lv))
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

void UIRSlider::interpPreset(t_float idx)
{
    Atom def[2] = { 0.f, 0.f };
    auto lv0 = PresetStorage::instance().listValueAt(presetId(), static_cast<int>(idx), AtomListView(def, 2));
    auto lv1 = PresetStorage::instance().listValueAt(presetId(), static_cast<int>(idx) + 1, AtomListView(def, 2));

    float k = (static_cast<float>(idx) - static_cast<int>(idx));
    onList(interp_lists(lv0, lv1, 2, k));
}

void UIRSlider::storePreset(size_t idx)
{
    PresetStorage::instance().setListValueAt(presetId(), idx, propValue());
}

void UIRSlider::adjustValues()
{
    if (prop_min < prop_max && vhigh_ < vlow_)
        std::swap(vlow_, vhigh_);
    else if (prop_min > prop_max && vhigh_ > vlow_)
        std::swap(vlow_, vhigh_);
}

void UIRSlider::redrawKnob()
{
    knob_layer_.invalidate();
    redraw();
}

void UIRSlider::output()
{
    Atom res[2];
    res[0] = vlow_;
    res[1] = vhigh_;
    listTo(0, AtomListView(res, 2));
    send(AtomListView(res, 2));
}

bool UIRSlider::setValue(const AtomListView& lv)
{
    if (lv.size() != 2) {
        UI_ERR << "min max values expected: " << lv;
        return false;
    }

    const size_t N = lv.size();

    if (N > 0 && lv[0].isFloat())
        vlow_ = clip_any<float>(lv[0].asFloat(), prop_min, prop_max);

    if (N > 1 && lv[1].isFloat())
        vhigh_ = clip_any<float>(lv[1].asFloat(), prop_min, prop_max);

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
    obj.outputMouseEvents(MouseEventsOutput::DEFAULT_OFF);

    obj.addFloatProperty("min", _("Minimum Value"), 0.f, &UIRSlider::prop_min, _("Bounds"));
    obj.addFloatProperty("max", _("Maximum Value"), 1.f, &UIRSlider::prop_max, _("Bounds"));
    obj.addProperty("knob_color", _("Knob Color"), DEFAULT_ACTIVE_COLOR, &UIRSlider::prop_color_knob);

    obj.addProperty("value", &UIRSlider::propValue, &UIRSlider::propSetValue);
    obj.addProperty("range", &UIRSlider::propRange, 0);
    obj.setPropertyMin("range", 0);
    obj.addProperty("low", &UIRSlider::propLow, &UIRSlider::propSetLow);
    obj.addProperty("high", &UIRSlider::propHigh, &UIRSlider::propSetHigh);

    obj.addMethod("set", &UIRSlider::propSetValue);
}

void setup_ui_rslider()
{
    UIRSlider::setup();
}

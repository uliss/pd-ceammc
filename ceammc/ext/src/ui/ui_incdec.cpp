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
#include "ui_incdec.h"
#include "ceammc_preset.h"
#include "ceammc_ui.h"

UIIncDec::UIIncDec()
    : prop_color_arrow(rgba_greydark)
    , prop_step(1)
    , value_(0)
    , mouse_down_(0)
{
    createOutlet();
}

void UIIncDec::okSize(t_rect* newrect)
{
    newrect->w = pd_clip_min(newrect->w, 15.);
    newrect->h = pd_clip_min(newrect->h, 15.);
}

void UIIncDec::paint()
{
    const auto r = rect();
    UIPainter p = bg_layer_.painter(r);

    if (!p)
        return;

    const float arrow_height = roundf(r.h / 2.f);

    // Background
    p.setColor(prop_color_arrow);

    if (mouse_down_ == 1)
        p.drawRect(0, 0, r.w, arrow_height);
    else if (mouse_down_ == -1)
        p.drawRect(0, arrow_height, r.w, arrow_height);

    p.fill();

    // Arrow Up //
    p.setColor(mouse_down_ == 1 ? prop_color_background : prop_color_arrow);

    const int arrow_p0_x = static_cast<int>(roundf(pd_clip_min(r.w * 0.2f, 2)));
    const int arrow_p1_x = r.w - arrow_p0_x;
    const int arrow_p2_x = static_cast<int>(roundf(r.w * 0.5f));

    const int arrow_p0_y = static_cast<int>(roundf(pd_clip_min(arrow_height * 0.2f, 2)));
    const int arrow_p1_y = arrow_p0_y;
    const int arrow_p2_y = arrow_height - arrow_p0_y;

    p.moveTo(arrow_p0_x, arrow_height - arrow_p0_y);
    p.drawLineTo(arrow_p1_x, arrow_height - arrow_p1_y);
    p.drawLineTo(arrow_p2_x, arrow_p0_y);
    p.drawLineTo(arrow_p0_x, arrow_height - arrow_p0_y);
    p.fill();

    // Arrow Down //
    p.setColor(mouse_down_ == -1 ? prop_color_background : prop_color_arrow);

    p.moveTo(arrow_p0_x, arrow_height + arrow_p0_y);
    p.drawLineTo(arrow_p1_x, arrow_height + arrow_p1_y);
    p.drawLineTo(arrow_p2_x, arrow_height + arrow_p2_y);
    p.drawLineTo(arrow_p0_x, arrow_height + arrow_p0_y);
    p.fill();

    // Middle Line //
    p.setColor(prop_color_border);
    p.setLineWidth(1);
    p.drawLine(0, arrow_height, r.w, arrow_height);
}

void UIIncDec::onBang()
{
    output();
}

void UIIncDec::onFloat(t_float f)
{
    propSetValue(f);
    output();
}

void UIIncDec::onMouseDown(t_object* view, const t_pt& pt, const t_pt& abs_pt, long modifiers)
{
    if (pt.y < height() / 2) {
        m_inc();
        mouse_down_ = 1;
    } else {
        m_dec();
        mouse_down_ = -1;
    }

    redrawBGLayer();
}

void UIIncDec::onMouseUp(t_object* view, const t_pt& pt, long modifiers)
{
    mouse_down_ = 0;
    redrawBGLayer();
}

void UIIncDec::m_inc()
{
    value_ += prop_step;
    redrawBGLayer();
    output();
}

void UIIncDec::m_dec()
{
    value_ -= prop_step;
    redrawBGLayer();
    output();
}

void UIIncDec::loadPreset(size_t idx)
{
    onFloat(PresetStorage::instance().floatValueAt(presetId(), idx, 0));
}

void UIIncDec::storePreset(size_t idx)
{
    PresetStorage::instance().setFloatValueAt(presetId(), idx, value_);
}

void UIIncDec::output()
{
    floatTo(0, value_);
    send(value_);
}

t_float UIIncDec::propValue() const
{
    return value_;
}

void UIIncDec::propSetValue(t_float f)
{
    value_ = f;
    redrawBGLayer();
}

void UIIncDec::setup()
{
    UIObjectFactory<UIIncDec> obj("ui.incdec");
    obj.setDefaultSize(15, 20);
    obj.hideLabelInner();

    obj.useBang();
    obj.useFloat();
    obj.usePresets();

    obj.useMouseEvents(UI_MOUSE_DOWN | UI_MOUSE_UP);

    obj.addProperty("arrow_color", _("Arrow Color"), DEFAULT_BORDER_COLOR, &UIIncDec::prop_color_arrow);
    obj.addFloatProperty("step", _("Step increment"), 1., &UIIncDec::prop_step, _("Main"));
    obj.setPropertyMin("step", 0);
    obj.addProperty("value", &UIIncDec::propValue, &UIIncDec::propSetValue);
    obj.addMethod("set", &UIIncDec::propSetValue);
    obj.addMethod("inc", &UIIncDec::m_inc);
    obj.addMethod("dec", &UIIncDec::m_dec);
}

void setup_ui_incdec()
{
    UIIncDec::setup();
}

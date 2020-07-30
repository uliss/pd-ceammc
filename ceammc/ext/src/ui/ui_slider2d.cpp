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

static const float KNOB_MIN_SIZE = 5.f;
static const float KNOB_MAX_SIZE = 20.f;
static const float KNOB_RATIO = 0.1f;
static const float KNOB_BORDER_WIDTH = 1.f;
static t_rgba KNOB_FILL = hex_to_rgba("#C0C0C0");
static t_rgba KNOB_BORDER = hex_to_rgba("#707070");
static t_rgba KNOB_FILL_ACTIVE = hex_to_rgba("#003070");
static t_rgba KNOB_BORDER_ACTIVE = hex_to_rgba("#00C0FF");
static t_rgba GUIDE_LINE_COLOR = hex_to_rgba("#00C0F0");
static const float GUIDE_LINE_WIDTH = 0.5f;

UISlider2D::UISlider2D()
    : prop_x_left(-1)
    , prop_x_right(1)
    , prop_y_top(1)
    , prop_y_bottom(-1)
    , prop_show_range(1)
    , prop_show_grid(0)
    , txt_font(gensym(FONT_FAMILY), FONT_SIZE_SMALL)
    , txt_xrange_(txt_font.font(), ColorRGBA::black(), ETEXT_UP_LEFT, ETEXT_JLEFT)
    , txt_yrange_(txt_font.font(), ColorRGBA::black(), ETEXT_DOWN_RIGHT, ETEXT_JRIGHT)
    , knob_layer_(asEBox(), gensym("knob_layer"))
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
    paintBackground();
    paintKnob();
}

void UISlider2D::onPropChange(t_symbol* prop_name)
{
    updateLabels();
    redrawAll();
}

void UISlider2D::paintBackground()
{
    const t_rect r = rect();

    UIPainter p = bg_layer_.painter(r);
    if (!p)
        return;

    if (prop_show_grid) {
        const float X_GRID_STEP = r.width / 10;
        const float Y_GRID_STEP = r.height / 10;

        // draw center cross
        p.setLineWidth(3);
        p.setColor(rgba_addContrast(prop_color_background, 0.1));
        p.drawLine(5 * X_GRID_STEP, -1, 5 * X_GRID_STEP, r.height + 2);
        p.drawLine(-1, 5 * Y_GRID_STEP, r.width + 2, 5 * Y_GRID_STEP);
        p.setLineWidth(1);
        p.setColor(rgba_addContrast(prop_color_background, -0.1));

        // draw horizontal lines
        for (int i = 1; i < 10; i++)
            p.drawLine(i * X_GRID_STEP, -1, i * X_GRID_STEP, r.height + 2);

        // draw vertical lines
        for (int i = 1; i < 10; i++)
            p.drawLine(-1, i * Y_GRID_STEP, r.width + 2, i * Y_GRID_STEP);
    }

    if (prop_show_range) {
        updateLabels();

        txt_xrange_.setColor(prop_color_border);
        txt_yrange_.setColor(prop_color_border);

        p.drawText(txt_xrange_);
        p.drawText(txt_yrange_);
    }
}

void UISlider2D::paintKnob()
{
    const t_rect r = rect();
    UIPainter p = knob_layer_.painter(r);

    if (p) {
        const float x = convert::lin2lin_clip<float>(x_pos_, prop_x_left, prop_x_right, 0, r.width);
        const float y = convert::lin2lin<float>(y_pos_, prop_y_top, prop_y_bottom, 0, r.height);

        // when grid shown - no guide lines are needed
        if (!prop_show_grid) {
            // guide lines
            p.setLineWidth(GUIDE_LINE_WIDTH);
            p.setColor(GUIDE_LINE_COLOR);
            p.drawLine(x, 0, x, r.height);
            p.drawLine(0, y, r.width, y);
        }

        // knob
        const float knobsize = clip<float>(KNOB_MIN_SIZE, KNOB_MAX_SIZE,
            std::min(r.height, r.width) * KNOB_RATIO);

        // knob border
        p.setLineWidth(KNOB_BORDER_WIDTH);
        p.setColor(mouse_down_ ? KNOB_FILL_ACTIVE : KNOB_FILL);
        p.drawRect(x - 0.5f * knobsize, y - 0.5f * knobsize, knobsize, knobsize);
        p.fillPreserve();
        p.setColor(mouse_down_ ? KNOB_BORDER_ACTIVE : KNOB_BORDER);
        p.stroke();
    }
}

void UISlider2D::onBang()
{
    output();
}

void UISlider2D::onList(const AtomList& lst)
{
    if (lst.size() != 2) {
        UI_ERR << "invalid list given: X Y expected";
        return;
    }

    if (!setRealValue(lst))
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

void UISlider2D::m_set(const AtomList& lst)
{
    if (lst.size() != 2) {
        UI_ERR << "invalid list given: set X Y expected";
        return;
    }

    if (!setRealValue(lst))
        return;

    redrawKnob();
}

void UISlider2D::m_move(const AtomList& lst)
{
    if (lst.size() != 2) {
        UI_ERR << "invalid data: X Y offset expected: " << lst;
        return;
    }

    t_float x, y;
    if (!lst[0].getFloat(&x) || !lst[1].getFloat(&y)) {
        UI_ERR << "float offsets are expected: " << lst;
        return;
    }

    if (!setRealValue(AtomList(realXValue() + x, realYValue() + y)))
        return;

    redrawKnob();
}

void UISlider2D::loadPreset(size_t idx)
{
    setRealValue(PresetStorage::instance().listValueAt(presetId(), idx, AtomList(0.f, 0.f)));
    redrawKnob();
    output();
}

void UISlider2D::storePreset(size_t idx)
{
    PresetStorage::instance().setListValueAt(presetId(), idx, realValue());
}

bool UISlider2D::setRealValue(const AtomList& lst)
{
    if (lst.size() != 2)
        return false;

    t_float x, y;
    if (!lst[0].getFloat(&x) || !lst[1].getFloat(&y)) {
        UI_ERR << "invalid value: " << lst;
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
    AtomList v = realValue();
    listTo(0, v);
    send(v);
}

AtomList UISlider2D::propXRange() const
{
    return AtomList(xRange());
}

AtomList UISlider2D::propYRange() const
{
    return AtomList(yRange());
}

AtomList UISlider2D::propXValue() const
{
    return AtomList(realXValue());
}

AtomList UISlider2D::propYValue() const
{
    return AtomList(realYValue());
}

void UISlider2D::propSetXValue(const AtomList& lst)
{
    t_float x;
    if (lst.empty() || !lst[0].getFloat(&x)) {
        UI_ERR << "x float value expected: " << lst;
        return;
    }

    if (xRange() == 0) {
        UI_ERR << "invalid x-range: " << xRange();
        return;
    }

    x_pos_ = clip_any<t_float>(x, prop_x_left, prop_x_right);

    redrawKnob();
}

void UISlider2D::propSetYValue(const AtomList& lst)
{
    t_float y;
    if (lst.empty() || !lst[0].getFloat(&y)) {
        UI_ERR << "y float value expected: " << lst;
        return;
    }

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

    obj.setDefaultSize(100, 100);

    obj.addFloatProperty("x_left", _("Left X-value"), -1, &UISlider2D::prop_x_left, "Bounds");
    obj.addFloatProperty("x_right", _("Right X-value"), 1, &UISlider2D::prop_x_right, "Bounds");
    obj.addFloatProperty("y_top", _("Top Y-value"), 1, &UISlider2D::prop_y_top, "Bounds");
    obj.addFloatProperty("y_bottom", _("Bottom Y-value"), -1, &UISlider2D::prop_y_bottom, "Bounds");

    obj.addProperty("x_range", &UISlider2D::propXRange, 0);
    obj.addProperty("y_range", &UISlider2D::propYRange, 0);
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
    knob_layer_.invalidate();
    redraw();
}

void UISlider2D::redrawAll()
{
    knob_layer_.invalidate();
    bg_layer_.invalidate();
    redraw();
}

void UISlider2D::setMouse(t_float x, t_float y)
{
    x_pos_ = convert::lin2lin_clip<t_float>(x, 0, width(), prop_x_left, prop_x_right);
    y_pos_ = convert::lin2lin_clip<t_float>(y, 0, height(), prop_y_top, prop_y_bottom);
}

void UISlider2D::updateLabels()
{
    char buf[64];
    snprintf(buf, 64, "X: [%g..%g]", prop_x_left, prop_x_right);

    const float xoff = 2 * zoom();
    const float yoff = 3 * zoom();
    txt_xrange_.set(buf, xoff, yoff, 100, 20);

    snprintf(buf, 64, "Y: [%g..%g]", prop_y_top, prop_y_bottom);
    txt_yrange_.set(buf, width() - xoff, height() - yoff, 100, 20);
}

void setup_ui_slider2d()
{
    UISlider2D::setup();
}

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

#include "ceammc_convert.h"
#include "ceammc_preset.h"
#include "ceammc_ui.h"
#include "ui_polar.h"

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

enum Sides {
    S_NORTH = 1,
    S_EAST = 0,
    S_SOUTH = 3,
    S_WEST = 2
};

static int sym2direction(t_symbol* s)
{
    switch (s->s_name[0]) {
    case 'N':
        return S_NORTH;
    case 'W':
        return S_WEST;
    case 'S':
        return S_SOUTH;
    case 'E':
    default:
        return S_EAST;
    }
}

static float direction2angle(t_symbol* s)
{
    return sym2direction(s) * -M_PI_2;
}

UIPolar::UIPolar()
    : txt_font(gensym(FONT_FAMILY), 7)
    , txt_radius_(txt_font.font(), KNOB_BORDER, ETEXT_UP_LEFT, ETEXT_JLEFT)
    , txt_angle_(txt_font.font(), KNOB_BORDER, ETEXT_UP_RIGHT, ETEXT_JRIGHT)
    , knob_layer_(asEBox(), gensym("knob_layer"))
    , radius_(1)
    , angle_(0)
    , prop_direction_(gensym("N"))
    , mouse_down_(false)
    , right_click_(false)
    , prop_clockwise_(1)
    , prop_use_deg_(0)
    , prop_positive_(0)
{
    createOutlet();
}

void UIPolar::okSize(t_rect* newrect)
{
    newrect->height = std::max<float>(30, newrect->height);
    newrect->width = std::max<float>(30, newrect->width);
}

void UIPolar::paint()
{
    paintBackground();
    paintKnob();
}

void UIPolar::onPropChange(t_symbol* prop_name)
{
    redrawAll();
}

void UIPolar::paintBackground()
{
    const t_rect bbox = rect();

    UIPainter p = bg_layer_.painter(bbox);
    if (!p)
        return;

    p.setColor(prop_color_border);
    const float r = bbox.width / 2;
    const float c = r;

    auto color = rgba_addContrast(prop_color_border, 0.2);

    // draw circles
    for (int i = 5; i > 0; i--) {
        p.setLineWidth(3);
        p.setColor(rgba_white);
        p.drawCircle(c, c, i * 0.2 * r);
        p.strokePreserve();

        p.setLineWidth(1);
        p.setColor(color);
        p.drawCircle(c, c, i * 0.2 * r);
        p.stroke();
    }

    // draw angle lines
    auto draw_fn = [&bbox, &p](float w) {
        const float k = (1 - M_SQRT1_2) * bbox.width * 0.5;
        p.setLineWidth(w);
        p.drawLine(0, bbox.width * 0.5, bbox.width, bbox.width * 0.5);
        p.drawLine(bbox.width * 0.5, 0, bbox.width * 0.5, bbox.height);
        p.drawLine(k, k, bbox.width - k, bbox.height - k);
        p.drawLine(k, bbox.height - k, bbox.width - k, k);
    };

    p.setColor(rgba_white);
    draw_fn(3);
    p.setColor(color);
    draw_fn(1);

    // draw 1, 0 line
    p.setColor(KNOB_BORDER_ACTIVE);
    p.setLineWidth(1);
    switch (sym2direction(prop_direction_)) {
    case S_NORTH:
        p.drawLine(c, c, c, 0);
        break;
    case S_EAST:
        p.drawLine(c, c, bbox.width, c);
        break;
    case S_SOUTH:
        p.drawLine(c, c, c, bbox.height);
        break;
    case S_WEST:
    default:
        p.drawLine(c, c, 0, c);
        break;
    }
}

void UIPolar::paintKnob()
{
    const t_rect bbox = rect();
    UIPainter p = knob_layer_.painter(bbox);

    if (p) {
        auto xy = convert::polar2cartesian<float>(radius_, angle_);
        const float x = convert::lin2lin<float>(xy.first, -1, 1, 0, bbox.width);
        const float y = convert::lin2lin<float>(xy.second, 1, -1, 0, bbox.height);

        // knob
        const float knobsize = clip<float>(KNOB_MIN_SIZE, KNOB_MAX_SIZE,
                                   std::min(bbox.height, bbox.width) * KNOB_RATIO)
            * 0.5;

        // knob border
        p.setLineWidth(KNOB_BORDER_WIDTH);
        p.setColor(mouse_down_ ? KNOB_FILL_ACTIVE : KNOB_FILL);
        p.drawCircle(x, y, knobsize);
        p.fillPreserve();
        p.setColor(mouse_down_ ? KNOB_BORDER_ACTIVE : KNOB_BORDER);
        p.stroke();

        char buf[60];
        snprintf(buf, sizeof(buf), "r:%.2f", radius_);
        txt_radius_.set(buf, 2, 2, 100, 100);
        p.drawText(txt_radius_);
        snprintf(buf, sizeof(buf), "α:%.2f", realAngle());
        txt_angle_.set(buf, bbox.width - 2, 2, 100, 100);
        p.drawText(txt_angle_);
    }
}

void UIPolar::onBang()
{
    output();
}

void UIPolar::onList(const AtomList& lst)
{
    if (lst.size() != 2) {
        UI_ERR << "invalid list given: RAD ANG expected";
        return;
    }

    if (!setRealValue(lst))
        return;

    redrawKnob();
    output();
}

void UIPolar::onPopup(t_symbol* menu_name, long item_idx)
{
    if (menu_name != gensym("popup"))
        return;

    switch (item_idx) {
    case 0:
        onList({ 0.f, 0.f });
        break;
    case 1:
        onList({ 1, 0.f });
        break;
    case 2:
        onList({ 1, M_PI });
        break;
    case 3:
        onList({ 1, M_PI_2 });
        break;
    case 4:
        onList({ 1, -M_PI_2 });
        break;
    }
}

void UIPolar::onMouseDown(t_object* view, const t_pt& pt, const t_pt& abs_pt, long modifiers)
{
    // right click
    if (modifiers & EMOD_RIGHT) {
        UIPopupMenu menu(asEObj(), "popup", abs_pt);
        menu.addItem(_("center"));
        menu.addItem(_("left center"));
        menu.addItem(_("right center"));
        menu.addItem(_("top center"));
        menu.addItem(_("bottom center"));
        right_click_ = true;
        return;
    }

    right_click_ = false;
    mouse_down_ = true;
    setMouse(pt.x, pt.y);
    redrawKnob();
    output();
}

void UIPolar::onMouseDrag(t_object* view, const t_pt& pt, long modifiers)
{
    setMouse(pt.x, pt.y);
    redrawKnob();
    output();
}

void UIPolar::onMouseUp(t_object* view, const t_pt& pt, long modifiers)
{
    if (right_click_) {
        right_click_ = false;
        return;
    }

    mouse_down_ = false;
    setMouse(pt.x, pt.y);
    redrawKnob();
    output();
}

void UIPolar::m_set(const AtomList& lst)
{
    if (lst.size() != 2) {
        UI_ERR << "invalid list given: set RAD ANG expected";
        return;
    }

    if (!setRealValue(lst))
        return;

    redrawKnob();
}

void UIPolar::loadPreset(size_t idx)
{
    setRealValue(PresetStorage::instance().listValueAt(presetId(), idx, AtomList(0.f, 0.f)));
    redrawKnob();
    output();
}

void UIPolar::storePreset(size_t idx)
{
    PresetStorage::instance().setListValueAt(presetId(), idx, realValue());
}

bool UIPolar::setRealValue(const AtomList& lst)
{
    if (lst.size() != 2)
        return false;

    float r, a;
    if (!lst[0].getFloat(&r) || !lst[1].getFloat(&a)) {
        UI_ERR << "invalid value: " << lst;
        return false;
    }

    if (r < 0 || r > 1)
        UI_DBG << "clipping radius into range: [0-1]";

    radius_ = clip<t_float>(r, 0, 1);
    angle_ = fromRealAngle(a);
    return true;
}

AtomList UIPolar::realValue() const
{
    return { radius_, realAngle() };
}

t_float UIPolar::realAngle() const
{
    t_float theta = (angle_ + direction2angle(prop_direction_)) * (prop_clockwise_ ? -1 : 1);

    if (prop_positive_)
        theta = wrapFloatMax<t_float>(theta, 2 * M_PI);
    else
        theta = wrapFloatMinMax<t_float>(theta, -M_PI, M_PI);

    return prop_use_deg_ ? (theta * (180 / M_PI)) : theta;
}

t_float UIPolar::fromRealAngle(t_float a) const
{
    if (prop_use_deg_)
        a *= (M_PI / 180);

    a *= (prop_clockwise_ ? -1 : 1);
    a -= direction2angle(prop_direction_);
    return a;
}

void UIPolar::output()
{
    AtomList v = realValue();
    listTo(0, v);
    send(v);
}

AtomList UIPolar::propRadius() const
{
    return AtomList(radius_);
}

AtomList UIPolar::propAngle() const
{
    return AtomList(realAngle());
}

void UIPolar::propSetRadius(const AtomList& lst)
{
    float r;

    if (lst.empty() || !lst[0].getFloat(&r)) {
        UI_ERR << "radius float value expected: " << lst;
        return;
    }

    radius_ = clip<t_float>(r, 0, 1);
    redrawKnob();
}

void UIPolar::propSetAngle(const AtomList& lst)
{
    float a;

    if (lst.empty() || !lst[0].getFloat(&a)) {
        UI_ERR << "angle float value expected: " << lst;
        return;
    }

    angle_ = fromRealAngle(a);
    redrawKnob();
}

void UIPolar::redrawKnob()
{
    knob_layer_.invalidate();
    redrawInnerArea();
}

void UIPolar::redrawAll()
{
    knob_layer_.invalidate();
    bg_layer_.invalidate();
    redraw();
}

void UIPolar::setMouse(float x, float y)
{
    auto r = rect();
    auto p = convert::cartesian2polar(
        convert::lin2lin<float>(x, 0, r.width, -1, 1),
        convert::lin2lin<float>(y, 0, r.height, 1, -1));
    radius_ = clip<float>(p.first, 0, 1);
    angle_ = p.second;
}

void setup_ui_polar()
{
    UIPolar::setup();
}

void UIPolar::setup()
{
    UIObjectFactory<UIPolar> obj("ui.polar", EBOX_GROWLINK);

    obj.setDefaultSize(100, 100);

    obj.addProperty("radius", &UIPolar::propRadius, &UIPolar::propSetRadius);
    obj.setPropertyRange("radius", 0, 1);
    obj.addProperty("angle", &UIPolar::propAngle, &UIPolar::propSetAngle);
    obj.addProperty("clockwise", _("Clockwise"), true, &UIPolar::prop_clockwise_, _("Main"));
    obj.addProperty("use_degrees", _("Use degrees"), false, &UIPolar::prop_use_deg_, _("Main"));
    obj.addProperty("positive", _("Positive output"), false, &UIPolar::prop_positive_, _("Main"));
    obj.addProperty("direction", _("Direction"), "N", &UIPolar::prop_direction_, "N W S E", _("Main"));

    obj.usePresets();
    obj.useList();
    obj.useBang();

    obj.useMouseEvents(UI_MOUSE_UP | UI_MOUSE_DOWN | UI_MOUSE_DRAG);

    obj.addMethod("set", &UIPolar::m_set);
}

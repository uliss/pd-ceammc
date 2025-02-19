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
#include <cmath>
#include <cstdio>
#include <ctime>
#include <random>

#include "ceammc_atomlist.h"
#include "ceammc_convert.h"

#include "ceammc_convert.h"
#include "ceammc_preset.h"
#include "ceammc_ui.h"
#include "cicm/Sources/egraphics.h"
#include "ui_polar.h"

static const float KNOB_MIN_SIZE = 5;
static const float KNOB_MAX_SIZE = 20;
static const float KNOB_RATIO = 0.1f;
static const float KNOB_BORDER_WIDTH = 1;
static t_rgba KNOB_FILL = hex_to_rgba("#C0C0C0");
static t_rgba KNOB_BORDER = hex_to_rgba("#707070");
static t_rgba KNOB_FILL_ACTIVE = hex_to_rgba("#003070");
static t_rgba KNOB_BORDER_ACTIVE = hex_to_rgba("#00C0FF");
static t_rgba GUIDE_LINE_COLOR = hex_to_rgba("#00C0F0");

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

static double direction2radians(t_symbol* s)
{
    return sym2direction(s) * M_PI_2;
}

static double direction2degrees(t_symbol* s)
{
    return sym2direction(s) * 90;
}

UIPolar::UIPolar()
    : txt_font(gensym(FONT_FAMILY), 7)
    , txt_radius_(txt_font.font(), KNOB_BORDER, ETEXT_UP_LEFT, ETEXT_JLEFT)
    , txt_angle_(txt_font.font(), KNOB_BORDER, ETEXT_UP_RIGHT, ETEXT_JRIGHT)
    , knob_layer_(asEBox(), gensym("knob_layer"))
    , radius_(0)
    , angle_(0)
    , prop_direction_(gensym("N"))
    , mouse_down_(false)
    , prop_clockwise_(1)
    , prop_radians_(0)
    , prop_positive_(0)
{
    createOutlet();

    initPopupMenu("polar",
        { { _("center"), [this](const t_pt&) { onList(AtomList { 0.f, 0.f }); } },
            { _("left center"), [this](const t_pt&) { onList(AtomList { 1, side2Angle(LEFT) }); } },
            { _("right center"), [this](const t_pt&) { onList(AtomList { 1, side2Angle(RIGHT) }); } },
            { _("top center"), [this](const t_pt&) { onList(AtomList { 1, side2Angle(TOP) }); } },
            { _("bottom center"), [this](const t_pt&) { onList(AtomList { 1, side2Angle(BOTTOM) }); } } });
}

t_float UIPolar::side2Angle(SideT side)
{
    t_float angle = 0;

    if (prop_radians_)
        angle = side * M_PI_2;
    else
        angle = side * 90;

    if (prop_clockwise_)
        angle = directionAngleOffset() - angle;
    else
        angle -= directionAngleOffset();

    return angle;
}

void UIPolar::okSize(t_rect* newrect)
{
    newrect->h = std::max<float>(30, newrect->h);
    newrect->w = std::max<float>(30, newrect->w);
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
    const float r = bbox.w / 2;
    const float c = r;

    const auto color = rgba_addContrast(prop_color_background, -0.1);
    const auto color2 = rgba_addContrast(prop_color_background, 0.1);

    // draw circles
    for (int i = 5; i > 0; i--) {
        p.setLineWidth(3);
        p.setColor(color2);
        p.drawCircle(c, c, i * 0.2 * r);
        p.strokePreserve();

        p.setLineWidth(1);
        p.setColor(color);
        p.drawCircle(c, c, i * 0.2 * r);
        p.stroke();
    }

    // draw angle lines
    auto draw_fn = [&bbox, &p](float w) {
        const float k = (1 - M_SQRT1_2) * bbox.w * 0.5;
        p.setLineWidth(w);
        p.drawLine(0, bbox.w * 0.5, bbox.w, bbox.w * 0.5);
        p.drawLine(bbox.w * 0.5, 0, bbox.w * 0.5, bbox.h);
        p.drawLine(k, k, bbox.w - k, bbox.h - k);
        p.drawLine(k, bbox.h - k, bbox.w - k, k);
    };

    p.setColor(color2);
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
        p.drawLine(c, c, bbox.w, c);
        break;
    case S_SOUTH:
        p.drawLine(c, c, c, bbox.h);
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
        float a = (prop_radians_) ? angle_ : convert::degree2rad(angle_);
        if (prop_clockwise_)
            a = -a;

        a += direction2radians(prop_direction_);

        auto xy = convert::polar2cartesian<float>(radius_, a);
        const float x = convert::lin2lin<float>(xy.first, -1, 1, 0, bbox.w);
        const float y = convert::lin2lin<float>(xy.second, 1, -1, 0, bbox.h);

        // knob
        const float knobsize = clip<float>(KNOB_MIN_SIZE, KNOB_MAX_SIZE,
                                   std::min(bbox.h, bbox.w) * KNOB_RATIO)
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
        txt_angle_.set(buf, bbox.w - 2, 2, 100, 100);
        p.drawText(txt_angle_);
    }
}

void UIPolar::onBang()
{
    output();
}

void UIPolar::onList(const AtomListView& lv)
{
    if (lv.size() != 2) {
        UI_ERR << "invalid list given: RAD ANG expected";
        return;
    }

    if (!setRealValue(lv))
        return;

    redrawKnob();
    output();
}

void UIPolar::onMouseWheel(const t_pt& pt, long modifiers, double delta)
{
    float k = 0.1;
    if (modifiers & EMOD_SHIFT)
        k = 0.025;

    radius_ = clip<float, 0, 1>(radius_ + delta * k);
    redrawKnob();
    output();
}

void UIPolar::onMouseDown(t_object* view, const t_pt& pt, const t_pt& abs_pt, long modifiers)
{
    mouse_down_ = true;
    setMouse(pt.x, pt.y, modifiers & EMOD_ALT);
    redrawKnob();
    output();
}

void UIPolar::onMouseDrag(t_object* view, const t_pt& pt, long modifiers)
{
    setMouse(pt.x, pt.y, modifiers & EMOD_ALT);
    redrawKnob();
    output();
}

void UIPolar::onMouseUp(t_object* view, const t_pt& pt, long modifiers)
{
    mouse_down_ = false;
    setMouse(pt.x, pt.y, modifiers & EMOD_ALT);
    redrawKnob();
    output();
}

void UIPolar::m_set(const AtomListView& lv)
{
    static std::default_random_engine eng(time(nullptr));
    static const t_float m_pi = std::acos(-1);

    auto SYM_RAND = gensym("random");

    const bool ok = (lv == SYM_RAND)
        || (lv.size() == 2 && lv[0].isFloat() && lv[1].isFloat());

    if (!ok) {
        UI_ERR << "invalid list given: set RAD ANG or set random expected: got: " << lv;
        return;
    }

    if (lv == SYM_RAND) {
        std::uniform_real_distribution<t_float> dist(0, 1);
        radius_ = dist(eng);
        angle_ = dist(eng) * (prop_radians_ ? m_pi : 360.0);
    } else if (!setRealValue(lv))
        return;

    redrawKnob();
}

void UIPolar::m_polar(const AtomListView& lv)
{
    if (lv.size() != 2) {
        UI_ERR << "invalid list given: polar RAD ANG expected";
        return;
    }

    m_set(lv);
    output();
}

void UIPolar::m_cartesian(const AtomListView& lv)
{
    if (lv.size() != 2) {
        UI_ERR << "invalid list given: polar RAD ANG expected";
        return;
    }

    t_float x, y;
    if (!lv[0].getFloat(&x) || !lv[1].getFloat(&y)) {
        UI_ERR << "invalid value: " << lv;
        return;
    }

    if (std::abs(x) > 1) {
        UI_ERR << "clipping x value into [-1, 1] range";
        x = clip<t_float, -1, 1>(x);
    }

    if (std::abs(y) > 1) {
        UI_ERR << "clipping y value into [-1, 1] range";
        y = clip<t_float, -1, 1>(y);
    }

    auto pos = convert::cartesian2polar<double>(x, y);

    radius_ = clip<t_float>(pos.first, 0, 1);
    if (!prop_radians_)
        angle_ = convert::rad2degree(pos.second);

    redrawKnob();
    output();
}

void UIPolar::m_rotate(t_float angle)
{
    angle_ += angle;
    redrawKnob();
    output();
}

void UIPolar::m_random()
{
    Atom s = gensym("random");
    m_set(AtomListView(s));
    output();
}

void UIPolar::loadPreset(size_t idx)
{
    Atom def[2] = { 0., 0. };
    auto lv = PresetStorage::instance().listValueAt(presetId(), idx, AtomListView(def, 2));
    setRealValue(lv);
    redrawKnob();
    output();
}

void UIPolar::storePreset(size_t idx)
{
    PresetStorage::instance().setListValueAt(presetId(), idx, realValue());
}

bool UIPolar::setRealValue(const AtomListView& lv)
{
    if (lv.size() != 2)
        return false;

    t_float r, a;
    if (!lv[0].getFloat(&r) || !lv[1].getFloat(&a)) {
        UI_ERR << "invalid value: " << lv;
        return false;
    }

    if (r < 0 || r > 1)
        UI_DBG << "clipping radius into range: [0-1]";

    radius_ = clip<t_float>(r, 0, 1);
    angle_ = a;
    return true;
}

AtomList UIPolar::realValue() const
{
    return { radius_, realAngle() };
}

t_float UIPolar::realAngle() const
{
    static const t_float m_pi = std::acos(t_float(-1));
    constexpr t_float EPSILON_K = 8;

    t_float res = 0;

    if (prop_radians_) {
        if (prop_positive_)
            res = wrapFloatMax<t_float>(angle_, 2 * m_pi);
        else
            res = wrapFloatMinMax<t_float>(angle_, -m_pi, m_pi);
    } else {
        if (prop_positive_)
            res = wrapFloatMax<t_float>(angle_, 360);
        else
            res = wrapFloatMinMax<t_float>(angle_, -180, 180);
    }

    if (std::fabs(res) < std::numeric_limits<t_float>::epsilon() * EPSILON_K)
        return 0;
    else
        return res;
}

t_float UIPolar::realRadius() const
{
    return radius_;
}

void UIPolar::output()
{
    Atom res[2];
    res[0] = radius_;
    res[1] = realAngle();
    AtomListView lv(res, 2);
    listTo(0, lv);
    send(lv);
}

t_float UIPolar::propRadius() const
{
    return radius_;
}

t_float UIPolar::propAngle() const
{
    return realAngle();
}

void UIPolar::propSetRadius(t_float r)
{
    radius_ = clip<t_float, 0, 1>(r);
    redrawKnob();
}

void UIPolar::propSetAngle(t_float a)
{
    angle_ = a;
    redrawKnob();
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

void UIPolar::interpPreset(t_float idx)
{
    Atom def[2] = { 0.f, 0.f };
    auto lv0 = PresetStorage::instance().listValueAt(presetId(), static_cast<int>(idx), AtomListView(def, 2));
    auto lv1 = PresetStorage::instance().listValueAt(presetId(), static_cast<int>(idx) + 1, AtomListView(def, 2));

    float k = (static_cast<float>(idx) - static_cast<int>(idx));
    onList(interp_lists(lv0, lv1, 2, k));
}

void UIPolar::redrawKnob()
{
    knob_layer_.invalidate();
    redraw();
}

void UIPolar::redrawAll()
{
    knob_layer_.invalidate();
    bg_layer_.invalidate();
    redraw();
}

double UIPolar::directionAngleOffset() const
{
    if (prop_radians_)
        return direction2radians(prop_direction_);
    else
        return direction2degrees(prop_direction_);
}

void UIPolar::setMouse(float x, float y, bool angleOnly)
{
    auto r = rect();
    auto p = convert::cartesian2polar<double>(
        convert::lin2lin<double>(x, 0, r.w, -1, 1),
        convert::lin2lin<double>(y, 0, r.h, 1, -1));

    if (!angleOnly)
        radius_ = clip<float, 0, 1>(p.first);

    float angle = p.second;
    if (!prop_radians_)
        angle = convert::rad2degree(angle);

    if (prop_clockwise_)
        angle = directionAngleOffset() - angle;
    else
        angle -= directionAngleOffset();

    angle_ = angle;
}

void setup_ui_polar()
{
    UIPolar::setup();
}

void UIPolar::setup()
{
    UIObjectFactory<UIPolar> obj("ui.polar", EBOX_GROWLINK);

    obj.setDefaultSize(100, 100);
    obj.useAnnotations();

    obj.addProperty("radius", &UIPolar::propRadius, &UIPolar::propSetRadius);
    obj.setPropertyRange("radius", 0, 1);
    obj.addProperty("angle", &UIPolar::propAngle, &UIPolar::propSetAngle);
    obj.addProperty("clockwise", _("Clockwise"), true, &UIPolar::prop_clockwise_, _("Main"));
    obj.addProperty("radians", _("Use radians"), false, &UIPolar::prop_radians_, _("Main"));
    obj.addProperty("positive", _("Positive output"), false, &UIPolar::prop_positive_, _("Main"));
    obj.addProperty("direction", _("Direction"), "N", &UIPolar::prop_direction_, "N W S E", _("Main"));

    obj.usePresets();
    obj.useList();
    obj.useBang();
    obj.usePopup();

    obj.useMouseEvents(UI_MOUSE_UP | UI_MOUSE_DOWN | UI_MOUSE_DRAG | UI_MOUSE_WHEEL);
    obj.outputMouseEvents(MouseEventsOutput::DEFAULT_OFF);

    obj.addMethod("set", &UIPolar::m_set);
    obj.addMethod("polar", &UIPolar::m_polar);
    obj.addMethod("cartesian", &UIPolar::m_cartesian);
    obj.addMethod("rotate", &UIPolar::m_rotate);
    obj.addMethod("random", &UIPolar::m_random);
}

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
#include "ui_number.h"
#include "ceammc_convert.h"
#include "ceammc_format.h"
#include "ceammc_preset.h"
#include "ceammc_ui.h"

#include <iomanip>
#include <iostream>
#include <sstream>

#ifdef __WIN32
static int font_size_corr(float h) {
    return std::floor(0.6 * h);
}
#else
static int font_size_corr(float h) {
    return 0.875 * h;
}
#endif

constexpr const char* DEFAULT_FONT_FAMILY = "Helvetica";
constexpr int DEFAULT_FONT_SIZE = 13;

UINumber::UINumber()
    : clock_(this, &UINumber::updateTextValue)
    , font_(gensym(DEFAULT_FONT_FAMILY), DEFAULT_FONT_SIZE)
    , text_(font_.font(), ColorRGBA::black(), ETEXT_LEFT, ETEXT_JLEFT, ETEXT_NOWRAP)
    , text_layer_(asEBox(), gensym("text_layer"))
    , enter_value_("...")
    , value_(0)
    , edit_mode_(MODE_DISPLAY)
    , last_update_time_(0)
    , drag_start_ypos_(0)
    , drag_increment_(1)
    , drag_start_value_(0)
    , prop_color_text(rgba_black)
    , prop_color_active(rgba_blue)
    , prop_min(-std::numeric_limits<float>::max())
    , prop_max(std::numeric_limits<float>::max())
    , prop_digits(8)
{
    text_value_[0] = '0';
    text_value_[1] = '\0';
    createOutlet();
}

void UINumber::okSize(t_rect* newrect)
{
    float border_min = std::min<float>(newrect->width, newrect->height);
    border_min = std::max<float>(10, border_min);
    newrect->height = border_min;
    newrect->width = pd_clip_min(newrect->width, sys_fontwidth(font_.size()) * 3 + 8);

    auto new_val_font_size = font_size_corr(newrect->height);
    font_.setSize(new_val_font_size);
}

void UINumber::onPropChange(t_symbol* prop_name)
{
    bg_layer_.invalidate();
    text_layer_.invalidate();
    redraw();
}

void UINumber::paint()
{
    drawBackground();
    drawValue();
}

void UINumber::drawBackground()
{
    const t_rect r = rect();
    UIPainter p = bg_layer_.painter(r);

    if (!p)
        return;

    const float width = r.height * 0.4f;
    p.setLineWidth(1);
    p.setColor(prop_color_border);
    p.moveTo(0, 0);
    p.drawLineTo(width, r.height * 0.5f);
    p.drawLineTo(0, r.height);
    p.stroke();
}

void UINumber::drawValue()
{
    const t_rect r = rect();
    UIPainter p = text_layer_.painter(r);

    if (!p)
        return;

    const float y_off = r.height * 0.5;
    const float x_off = std::max<float>(y_off, 5) + 2;

    switch (edit_mode_) {
    case MODE_DISPLAY:
        text_.setColor(prop_color_text);
        text_.set(text_value_, x_off, y_off, 0, 0);
        break;
    case MODE_WAIT_INPUT:
        text_.setColor(prop_color_active);
        text_.set(text_value_, x_off, y_off, 0, 0);
        break;
    case MODE_DO_INPUT:
        text_.setColor(prop_color_active);
        text_.set(enter_value_.c_str(), x_off, y_off, 0, 0);
        break;
    }

    p.drawText(text_);
}

void UINumber::onBang()
{
    output();
}

void UINumber::onFloat(t_float f)
{
    setValue(f);
    output();
}

void UINumber::setValue(t_float f)
{
    value_ = clip<t_float>(f, prop_min, prop_max);

    // if we get quick serie of numbers
    // draw not frequently then every 20 ms
    if (clock_gettimesince(last_update_time_) > 30) {
        last_update_time_ = clock_getlogicaltime();
        updateTextValue();
    } else {
        // but we need to draw last number in quick serie
        // it drawn delayed every time
        clock_.delay(20);
    }
}

void UINumber::onDblClick(t_object* view, const t_pt& pt, long modifiers)
{
    switch (edit_mode_) {
    case MODE_DISPLAY:
        edit_mode_ = MODE_WAIT_INPUT;
        enter_value_ = "";
        break;
        // fall thru
    case MODE_WAIT_INPUT:
    case MODE_DO_INPUT:
        edit_mode_ = MODE_DISPLAY;
        break;
    }

    text_layer_.invalidate();
    redraw();
}

void UINumber::onKey(int k, long modifiers)
{
    constexpr int KEY_UP = 0xFF52;
    constexpr int KEY_DOWN = 0xFF54;

    if (edit_mode_ == MODE_DISPLAY) {
        if (k == '-' || k == KEY_DOWN)
            onFloat(value_ - 1);
        else if (k == '+' || k == KEY_UP)
            onFloat(value_ + 1);

        return;
    }

    // switch to input
    edit_mode_ = MODE_DO_INPUT;

    if (k == '-' && enter_value_.empty()) {
        enter_value_.push_back('-');
    } else if (k == '.') {
        size_t len = enter_value_.size();
        float f = atof(enter_value_.c_str());
        int i = atoi(enter_value_.c_str());

        bool no_middle_dot = ((f - i) == 0);
        bool ends_with_dot = len > 0 ? (enter_value_[len - 1] == '.') : false;

        if (no_middle_dot && !ends_with_dot)
            enter_value_.push_back(k);
    } else if (isdigit(k))
        enter_value_.push_back(k);

    redrawValue();
}

void UINumber::onKeyFilter(int k, long modifiers)
{
    if (edit_mode_ == MODE_DISPLAY)
        return;

    // switch to input
    edit_mode_ = MODE_DO_INPUT;

    if (k == EKEY_DEL) {
        if (!enter_value_.empty()) {
            enter_value_.resize(enter_value_.size() - 1);
            redrawValue();
        }
    } else if (k == EKEY_TAB || k == EKEY_ENTER) {
        std::istringstream is(enter_value_);
        t_float v;
        is >> v;

        edit_mode_ = MODE_DISPLAY;
        onFloat(is ? v : 0);
    } else if (k == EKEY_ESC) {
        edit_mode_ = MODE_DISPLAY;
        enter_value_ = "";
        redrawValue();
    }
}

void UINumber::onMouseDown(t_object* view, const t_pt& pt, const t_pt& abs_pt, long modifiers)
{
    drag_start_ypos_ = pt.y;
    drag_start_value_ = value_;
}

void UINumber::onMouseDrag(t_object* view, const t_pt& pt, long modifiers)
{
    constexpr t_float SMALL_INCR = 0.01;
    constexpr t_float BIG_INCR = 10;

    edit_mode_ = MODE_DISPLAY;

    float inc = (drag_start_ypos_ - pt.y) * drag_increment_;

    if (modifiers & EMOD_SHIFT)
        inc *= SMALL_INCR;
    if (modifiers & EMOD_CTRL)
        inc *= BIG_INCR;

    onFloat(drag_start_value_ + inc);
}

void UINumber::onMouseLeave(t_object* view, const t_pt& pt, long modifiers)
{
    edit_mode_ = MODE_DISPLAY;
    redrawValue();
}

void UINumber::loadPreset(size_t idx)
{
    onFloat(PresetStorage::instance().floatValueAt(presetId(), idx));
}

void UINumber::storePreset(size_t idx)
{
    PresetStorage::instance().setFloatValueAt(presetId(), idx, value_);
}

void UINumber::interpPreset(t_float idx)
{
    onFloat(PresetStorage::instance().floatValueAt(presetId(), idx));
}

void UINumber::m_plus(t_float f)
{
    setValue(value_ + f);
}

void UINumber::m_minus(t_float f)
{
    setValue(value_ - f);
}

void UINumber::m_mul(t_float f)
{
    setValue(value_ * f);
}

void UINumber::m_div(t_float f)
{
    if (f == 0.f) {
        UI_ERR << "division by zero";
        return;
    }

    setValue(value_ / f);
}

void UINumber::m_increment()
{
    setValue(value_ + 1);
}

void UINumber::m_decrement()
{
    setValue(value_ - 1);
}

void UINumber::setup()
{
    UIObjectFactory<UINumber> obj("ui.number");
    obj.addAlias("ui.n");
    obj.usePresets();
    obj.useBang();
    obj.useFloat();
    obj.useMouseEvents(UI_MOUSE_DBL_CLICK | UI_MOUSE_DRAG | UI_MOUSE_DOWN | UI_MOUSE_LEAVE);
    obj.useKeys();
    obj.hideLabelInner();

    obj.setDefaultSize(53, 15);

    obj.addFloatProperty("min", _("Minimum Value"), -std::numeric_limits<float>::max(), &UINumber::prop_min, "Bounds");
    obj.addFloatProperty("max", _("Maximum Value"), std::numeric_limits<float>::max(), &UINumber::prop_max, "Bounds");

    obj.addProperty("digits", _("Digits"), -1, &UINumber::prop_digits);
    obj.setPropertyMin("digits", -1);
    obj.setPropertyMax("digits", 9);
    obj.setPropertyCategory("digits", _("Main"));

    obj.addProperty(PROP_ACTIVE_COLOR, _("Active Color"), DEFAULT_ACTIVE_COLOR, &UINumber::prop_color_active);
    obj.addProperty("text_color", _("Text color"), DEFAULT_TEXT_COLOR, &UINumber::prop_color_text);

    obj.addProperty("value", &UINumber::value, &UINumber::setValue);

    obj.addMethod("+", &UINumber::m_plus);
    obj.addMethod("-", &UINumber::m_minus);
    obj.addMethod("*", &UINumber::m_mul);
    obj.addMethod("/", &UINumber::m_div);
    obj.addMethod("++", &UINumber::m_increment);
    obj.addMethod("--", &UINumber::m_decrement);
    obj.addMethod("set", &UINumber::setValue);
}

void UINumber::output()
{
    floatTo(0, value_);
    send(value_);
}

void UINumber::redrawValue()
{
    text_layer_.invalidate();
    redraw();
}

void UINumber::updateTextValue()
{
    if (prop_digits < 0)
        snprintf(text_value_, sizeof(text_value_), sizeof(t_float) == 8 ? "%.17g" : "%.9g", value_);
    else if (prop_digits == 0)
        snprintf(text_value_, sizeof(text_value_), "%ld", long(value_));
    else {
        char fmt[] = "%.?f";
        fmt[2] = char((prop_digits % 9) + int('0'));
        snprintf(text_value_, sizeof(text_value_), fmt, value_);
    }

    redrawValue();
}

void setup_ui_number()
{
    UINumber::setup();
}

/*****************************************************************************
 * Copyright 2018 Serge Poltavsky, Alex Nadzharov. All rights reserved.
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

#include "ceammc_abstractdata.h"
#include "ceammc_atomlist.h"
#include "ceammc_format.h"

#include "ceammc_ui.h"
#include "ui_display.h"

using namespace ceammc;

static t_symbol* SYM_DATA_TYPE;
static t_symbol* SYM_PROP_SIZE;
static t_symbol* SYM_SIZE;
static t_rgba COLOR_LIST_TYPE = hex_to_rgba("#00A0C0");
static t_rgba COLOR_FLOAT_TYPE = hex_to_rgba("#E000A0");
static t_rgba COLOR_SYMBOL_TYPE = hex_to_rgba("#A0E000");
static t_rgba COLOR_DATA_TYPE = hex_to_rgba("#F07000");
static t_rgba COLOR_DEFAULT_TYPE = hex_to_rgba("#909090");
static t_rgba COLOR_PROPERTY_TYPE = hex_to_rgba("#00E0A0");
static t_rgba COLOR_BANG_TYPE = hex_to_rgba("#F03060");

#ifdef __APPLE__
static const int TYPE_WIDTH = 45;
#elif __WIN32
static const int TYPE_WIDTH = 58;
#else
static const int TYPE_WIDTH = 50;
#endif

// macosx, win - checked
static const int TEXT_XPAD = 3;
static const int TEXT_YPAD = 2;

static inline const t_rgba& msg_color(UIMessageType type)
{
    switch (type) {
    case MSG_TYPE_BANG:
        return COLOR_BANG_TYPE;
    case MSG_TYPE_FLOAT:
        return COLOR_FLOAT_TYPE;
    case MSG_TYPE_SYMBOL:
        return COLOR_SYMBOL_TYPE;
    case MSG_TYPE_PROPERTY:
        return COLOR_PROPERTY_TYPE;
    case MSG_TYPE_LIST:
        return COLOR_LIST_TYPE;
    case MSG_TYPE_DATA:
        return COLOR_DATA_TYPE;
    case MSG_TYPE_ANY:
    default:
        return COLOR_DEFAULT_TYPE;
    }
}

UIDisplay::UIDisplay()
    : prop_display_events(1)
    , prop_display_type(0)
    , prop_auto_size(1)
    , prop_text_color(rgba_black)
    , prop_active_color(rgba_white)
    , font_(gensym(FONT_FAMILY), FONT_SIZE)
    , txt_value_(font_.font(), ColorRGBA::black(), ETEXT_UP_LEFT, ETEXT_JLEFT, ETEXT_WRAP)
    , txt_type_(font_.font(), ColorRGBA::black(), ETEXT_UP_LEFT, ETEXT_JLEFT, ETEXT_NOWRAP)
    , msg_type_txt_(gensym("..."))
    , timer_(this, &UIDisplay::onClock)
    , last_update_(clock_getlogicaltime())
    , type_width_(-1)
    , on_bang_(false)
    , msg_type_(MSG_TYPE_ANY)
{
    msg_txt_.reserve(32);
}

void UIDisplay::init(t_symbol* name, const AtomList& args, bool usePresets)
{
    UIObject::init(name, args, usePresets);

    if (name == gensym("ui.dt"))
        prop_display_type = 1;
}

void UIDisplay::paint()
{
    const t_rect r = rect();

    UIPainter p = bg_layer_.painter(r);

    if (p) {
        if (prop_display_events) {
            p.setColor(on_bang_ ? prop_active_color : prop_color_background);
            p.setLineWidth(0);
            p.drawRect(0, 0, r.width, r.height);
            p.fill();
        }

        float x_offset = 0;

        if (prop_display_type) {
            p.setColor(msg_color(msg_type_));
            p.drawRect(0, 0, type_width_ * zoom(), r.height);
            p.fill();

            txt_type_.set(msg_type_txt_->s_name, 0 + TEXT_XPAD, 0 + TEXT_YPAD, type_width_ - TEXT_XPAD, r.height);
            txt_type_.setColor(prop_text_color);
            p.drawText(txt_type_);

            x_offset += type_width_ * zoom();
        }

        txt_value_.set(msg_txt_.c_str(),
            x_offset + TEXT_XPAD,
            0 + TEXT_YPAD,
            r.width - 2 * TEXT_XPAD - x_offset, r.height);
        txt_value_.setColor(prop_text_color);
        p.drawText(txt_value_);
    }
}

void UIDisplay::okSize(t_rect* newrect)
{
    float min_width = 40;
    if (prop_display_type != 0)
        min_width += TYPE_WIDTH;

    newrect->width = pd_clip_min(newrect->width, min_width);
    newrect->height = pd_clip_min(newrect->height, 18);
}

void UIDisplay::onBang()
{
    msg_txt_ = "";
    msg_type_txt_ = &s_bang;
    msg_type_ = MSG_TYPE_BANG;

    flash();
    update();
}

void UIDisplay::appendFloatToText(t_float f)
{
    char buf[48];

    if (prop_int_hex_ && std::round(f) == f) { // int
        if (f >= 0)
            snprintf(buf, sizeof(buf), "0x%0*lX", prop_hex_format_width_, static_cast<unsigned long>(f));
        else
            snprintf(buf, sizeof(buf), "-0x%0*lX", prop_hex_format_width_, static_cast<unsigned long>(-f));

        msg_txt_ += buf;
    } else {
        if (prop_float_precision_ < 0) {
            if (sizeof(t_float) == sizeof(float))
                snprintf(buf, sizeof(buf), "%.9g", f);
            else
                snprintf(buf, sizeof(buf), "%.17g", f);

            msg_txt_ += buf;
        } else if (prop_float_precision_ == 0) {
            msg_txt_ += std::to_string((t_int)f);
        } else {
            snprintf(buf, sizeof(buf), "%.*f", prop_float_precision_, f);
            msg_txt_ += buf;
        }
    }
}

void UIDisplay::onFloat(t_float f)
{
    msg_txt_.clear();
    appendFloatToText(f);
    msg_type_txt_ = &s_float;
    msg_type_ = MSG_TYPE_FLOAT;

    flash();
    update();
}

void UIDisplay::onSymbol(t_symbol* s)
{
    msg_txt_ = s->s_name;
    msg_type_txt_ = &s_symbol;
    msg_type_ = MSG_TYPE_SYMBOL;

    flash();
    update();
}

void UIDisplay::setMessage(UIMessageType t, t_symbol* s, const AtomListView& lst)
{
    msg_type_ = t;
    msg_type_txt_ = s;

    msg_txt_.clear();

    for (auto& a : lst) {
        // space separator
        if (!msg_txt_.empty())
            msg_txt_ += ' ';

        if (a.isFloat())
            appendFloatToText(a.asT<t_float>());
        else if (a.isSymbol())
            msg_txt_ += a.asT<t_symbol*>()->s_name;
        else if (a.isData())
            msg_txt_ += a.asData()->toString();
        else
            msg_txt_ += to_string(a);
    }
}

void UIDisplay::onList(const AtomListView& lst)
{
    if (lst.isData())
        setMessage(MSG_TYPE_DATA, gensym(lst[0].asData()->typeName().c_str()), lst);
    else
        setMessage(MSG_TYPE_LIST, &s_list, lst);

    flash();
    update();
}

void UIDisplay::onAny(t_symbol* s, const AtomListView& lst)
{
    setMessage(MSG_TYPE_ANY, s, lst);
    flash();
    update();
}

void UIDisplay::onProperty(t_symbol* s, const AtomListView& lst)
{
    if (s == SYM_PROP_SIZE && asEBox()->b_resize) {
        eclass_attr_setter(asPdObject(), SYM_SIZE, lst.size(), lst.toPdData());
        return;
    }

    setMessage(MSG_TYPE_PROPERTY, s, lst);
    flash();
    update();
}

void UIDisplay::onDblClick(t_object* view, const t_pt& pt, long modifiers)
{
    prop_display_type = !prop_display_type;
    update();
}

const std::string& UIDisplay::text() const
{
    return msg_txt_;
}

const std::string UIDisplay::type() const
{
    return msg_type_txt_->s_name;
}

const char* UIDisplay::annotateInlet(int /*n*/) const
{
    return "any message";
}

void UIDisplay::setup()
{
    UIObjectFactory<UIDisplay> obj("ui.display");
    obj.addAlias("ui.d");
    obj.addAlias("ui.dt");
    obj.hideLabel();
    obj.useAnnotations();

    obj.setDefaultSize(150, 18);

    obj.hideProperty("send");
    obj.addProperty("display_events", _("Display events"), true, &UIDisplay::prop_display_events, _("Main"));
    obj.addProperty("display_type", _("Display type"), false, &UIDisplay::prop_display_type, _("Main"));
    obj.addProperty("auto_size", _("Auto size"), true, &UIDisplay::prop_auto_size, _("Main"));
    obj.addProperty(PROP_TEXT_COLOR, _("Text Color"), DEFAULT_TEXT_COLOR, &UIDisplay::prop_text_color);
    obj.addProperty(PROP_ACTIVE_COLOR, _("Active Color"), DEFAULT_ACTIVE_COLOR, &UIDisplay::prop_active_color);
    obj.addIntProperty("float_width", _("Float precision"), -1, &UIDisplay::prop_float_precision_, _("Main"));
    obj.setPropertyRange("float_width", -1, 17);

    obj.addBoolProperty("hex", _("Integer in hex format"), false, &UIDisplay::prop_int_hex_, _("Main"));
    obj.addIntProperty("hex_width", _("Hex format width"), 2, &UIDisplay::prop_hex_format_width_, _("Main"));
    obj.setPropertyRange("hex_width", 0, 16);

    obj.setPropertyRedirect("active_color");
    obj.setPropertyRedirect("auto_size");
    obj.setPropertyRedirect("background_color");
    obj.setPropertyRedirect("border_color");
    obj.setPropertyRedirect("display_events");
    obj.setPropertyRedirect("display_type");
    obj.setPropertyRedirect("float_width");
    obj.setPropertyRedirect("fontname");
    obj.setPropertyRedirect("fontsize");
    obj.setPropertyRedirect("fontslant");
    obj.setPropertyRedirect("fontweight");
    obj.setPropertyRedirect("intbase");
    obj.setPropertyRedirect("pinned");
    obj.setPropertyRedirect("receive");
    obj.setPropertyRedirect("send");
    obj.setPropertyRedirect("send");
    obj.setPropertyRedirect("size");
    obj.setPropertyRedirect("text_color");

    obj.useBang();
    obj.useSymbol();
    obj.useFloat();
    obj.useList();
    obj.useAny();
    obj.useMouseEvents(UI_MOUSE_DBL_CLICK);
}

void UIDisplay::onClock()
{
    on_bang_ = false;
    redrawAll();
}

void UIDisplay::update()
{
    if (clock_gettimesince(last_update_) < 30)
        return;

    last_update_ = clock_getlogicaltime();

    if (prop_display_type) {
        const bool calc_type_wd = (msg_type_ == MSG_TYPE_ANY || msg_type_ == MSG_TYPE_DATA);

        if (calc_type_wd)
            type_width_ = std::max<int>(TYPE_WIDTH, strlen(msg_type_txt_->s_name) * 7) + 3;
        else
            type_width_ = TYPE_WIDTH;

    } else {
        type_width_ = 0;
    }

    if (prop_auto_size) {
        float w = msg_txt_.size() * 8 + type_width_ + 7;
        float h = int(w / 250) * 15 + 15;
        w = std::min(std::max(w, 20.f), 250.f); // 20 <= w <= 250
        resize(w, h);
    }

    redrawAll();
}

void UIDisplay::flash()
{
    if (prop_display_events) {
        on_bang_ = true;
        timer_.delay(100);
    }
}

void UIDisplay::redrawAll()
{
    bg_layer_.invalidate();
    redraw();
}

void setup_ui_display()
{
    SYM_DATA_TYPE = gensym("data");
    SYM_PROP_SIZE = gensym("@size");
    SYM_SIZE = gensym("size");

    UIDisplay::setup();
}

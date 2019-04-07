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

#include "ceammc_atomlist.h"
#include "ceammc_format.h"

#include "ceammc_ui.h"
#include "ui_display.h"

using namespace ceammc;

static t_symbol* SYM_DATA_TYPE = gensym("data");
static t_symbol* SYM_PROP_SIZE = gensym("@size");
static t_symbol* SYM_SIZE = gensym("size");
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

static const int TEXT_XPAD = 3;

#ifdef __WIN32
static const int TEXT_YPAD = 0;
#else
static const int TEXT_YPAD = 2;
#endif

static inline const t_rgba& msg_color(t_symbol* s_type)
{
    if (s_type == &s_list)
        return COLOR_LIST_TYPE;
    else if (s_type == &s_float)
        return COLOR_FLOAT_TYPE;
    else if (s_type == &s_bang)
        return COLOR_BANG_TYPE;
    else if (s_type == &s_symbol)
        return COLOR_SYMBOL_TYPE;
    else if (s_type == SYM_DATA_TYPE)
        return COLOR_DATA_TYPE;
    else if (s_type->s_name[0] == '@')
        return COLOR_PROPERTY_TYPE;
    else
        return COLOR_DEFAULT_TYPE;
}

UIDisplay::UIDisplay()
    : prop_display_events(1)
    , prop_display_type(0)
    , prop_auto_size(1)
    , on_bang_(false)
    , type_width_(-1)
    , prop_active_color(rgba_white)
    , prop_text_color(rgba_black)
    , font_(gensym(FONT_FAMILY), FONT_SIZE)
    , txt_value_(font_.font(), ColorRGBA::black(), ETEXT_UP_LEFT, ETEXT_JLEFT, ETEXT_WRAP)
    , txt_type_(font_.font(), ColorRGBA::black(), ETEXT_UP_LEFT, ETEXT_JLEFT, ETEXT_NOWRAP)
    , msg_type_(gensym("..."))
    , timer_(this, &UIDisplay::onClock)
    , last_update_(clock_getlogicaltime())
{
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

            txt_type_.set(msg_type_->s_name, 0 + TEXT_XPAD, 0 + TEXT_YPAD, type_width_ - TEXT_XPAD, r.height);
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
    msg_type_ = &s_bang;

    flash();
    update();
}

void UIDisplay::onFloat(t_float f)
{
    char buf[64];
    snprintf(buf, 63, "%g", f);

    msg_txt_ = buf;
    msg_type_ = &s_float;

    flash();
    update();
}

void UIDisplay::onSymbol(t_symbol* s)
{
    msg_txt_ = s->s_name;
    msg_type_ = &s_symbol;

    flash();
    update();
}

void UIDisplay::onList(const AtomList& lst)
{
    onAny(&s_list, lst);
}

void UIDisplay::onAny(t_symbol* s, const AtomList& lst)
{
    msg_txt_ = to_string(lst);
    msg_type_ = s;

    if (lst.size() == 1 && lst[0].isData())
        msg_type_ = SYM_DATA_TYPE;

    flash();
    update();
}

void UIDisplay::onProperty(t_symbol* s, const AtomList& lst)
{
    if (s == SYM_PROP_SIZE && asEBox()->b_resize) {
        eclass_attr_setter(asPdObject(), SYM_SIZE, lst.size(), lst.toPdData());
        return;
    }

    onAny(s, lst);
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
    return msg_type_->s_name;
}

void UIDisplay::setup()
{
    UIObjectFactory<UIDisplay> obj("ui.display");
    obj.addAlias("ui.d");
    obj.hideLabel();

    obj.setDefaultSize(150, 18);

    obj.hideProperty("send");
    obj.addProperty("display_events", _("Display events"), true, &UIDisplay::prop_display_events);
    obj.addProperty("display_type", _("Display type"), false, &UIDisplay::prop_display_type);
    obj.addProperty("auto_size", _("Auto size"), true, &UIDisplay::prop_auto_size);
    obj.addProperty(PROP_TEXT_COLOR, _("Text Color"), DEFAULT_TEXT_COLOR, &UIDisplay::prop_text_color);
    obj.addProperty(PROP_ACTIVE_COLOR, _("Active Color"), DEFAULT_ACTIVE_COLOR, &UIDisplay::prop_active_color);

    obj.setPropertyRedirect("send");
    obj.setPropertyRedirect("size");
    obj.setPropertyRedirect("fontname");
    obj.setPropertyRedirect("fontweight");
    obj.setPropertyRedirect("fontslant");
    obj.setPropertyRedirect("fontsize");
    obj.setPropertyRedirect("receive");
    obj.setPropertyRedirect("send");
    obj.setPropertyRedirect("pinned");
    obj.setPropertyRedirect("background_color");
    obj.setPropertyRedirect("border_color");
    obj.setPropertyRedirect("text_color");
    obj.setPropertyRedirect("active_color");
    obj.setPropertyRedirect("display_events");
    obj.setPropertyRedirect("display_type");
    obj.setPropertyRedirect("auto_size");

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
        const bool calc_type_wd = (msg_type_ != &s_float
            && msg_type_ != &s_bang
            && msg_type_ != &s_symbol
            && msg_type_ != &s_list
            && msg_type_ != SYM_DATA_TYPE);

        if (calc_type_wd)
            type_width_ = std::max<int>(TYPE_WIDTH, strlen(msg_type_->s_name) * 7) + 3;
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
    UIDisplay::setup();
}

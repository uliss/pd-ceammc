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
#include "ui_display.tcl.h"

using namespace ceammc;

static t_symbol* SYM_DATA_TYPE;
static t_symbol* SYM_PROP_SIZE;
static t_symbol* SYM_SIZE;
static t_symbol* SYM_DEFAULT;
static t_symbol* SYM_UI_DT;

static t_rgba COLOR_LIST_TYPE = hex_to_rgba("#00A0C0");
static t_rgba COLOR_FLOAT_TYPE = hex_to_rgba("#E000A0");
static t_rgba COLOR_SYMBOL_TYPE = hex_to_rgba("#A0E000");
static t_rgba COLOR_DATA_TYPE = hex_to_rgba("#F07000");
static t_rgba COLOR_DEFAULT_TYPE = hex_to_rgba("#909090");
static t_rgba COLOR_PROPERTY_TYPE = hex_to_rgba("#00E0A0");
static t_rgba COLOR_BANG_TYPE = hex_to_rgba("#F03060");

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

class AutoGuard {
    bool& v_;

public:
    AutoGuard(bool& v)
        : v_(v)
    {
        v_ = true;
    }

    ~AutoGuard() { v_ = false; }
};

UIDisplay::UIDisplay()
    : prop_display_events(1)
    , prop_display_type(0)
    , prop_auto_size(1)
    , prop_text_color(rgba_black)
    , prop_active_color(rgba_white)
    , timer_(this, &UIDisplay::onClock)
    , last_update_(clock_getlogicaltime())
    , msg_type_(SYM_DEFAULT)
    , on_bang_(false)
    , type_(MSG_TYPE_ANY)
{
    msg_txt_.reserve(32);

    char buf[64];
    snprintf(buf, 64, "r%p", this);
    rid_ = gensym(buf);
    pd_bind(asPd(), rid_);
}

UIDisplay::~UIDisplay()
{
    pd_unbind(asPd(), rid_);
}

void UIDisplay::init(t_symbol* name, const AtomList& args, bool usePresets)
{
    UIObject::init(name, args, usePresets);

    prop_display_type = (name == SYM_UI_DT);
}

void UIDisplay::paint()
{
    sys_vgui("ui::display_update %s %lx %s %d %d %d %d "
             "#%6.6x #%6.6x #%6.6x #%6.6x "
             "%d {%s} {%s}\n",
        asEBox()->b_canvas_id->s_name, asEBox(), rid_->s_name,
        (int)width(), (int)height(), (int)zoom(), (int)auto_,
        rgba_to_hex_int(prop_color_border),
        rgba_to_hex_int(on_bang_ ? prop_active_color : prop_color_background),
        rgba_to_hex_int(prop_text_color),
        rgba_to_hex_int(msg_color(type_)),
        prop_display_type, msg_type_->s_name,
        msg_txt_.c_str());
}

void UIDisplay::okSize(t_rect* newrect)
{
    float min_width = 32;

    newrect->width = pd_clip_min(newrect->width, min_width);
    newrect->height = pd_clip_min(newrect->height, 18);
}

void UIDisplay::onBang()
{
    AutoGuard g(auto_);
    msg_txt_ = "";
    msg_type_ = &s_bang;
    type_ = MSG_TYPE_BANG;

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
    AutoGuard g(auto_);

    msg_txt_.clear();
    appendFloatToText(f);
    msg_type_ = &s_float;
    type_ = MSG_TYPE_FLOAT;

    flash();
    update();
}

void UIDisplay::onSymbol(t_symbol* s)
{
    AutoGuard g(auto_);

    msg_txt_ = s->s_name;
    msg_type_ = &s_symbol;
    type_ = MSG_TYPE_SYMBOL;

    flash();
    update();
}

void UIDisplay::setMessage(UIMessageType t, t_symbol* s, const AtomListView& lst)
{
    type_ = t;
    msg_type_ = s;

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
    AutoGuard g(auto_);

    if (lst.isData())
        setMessage(MSG_TYPE_DATA, gensym(lst[0].asData()->typeName().c_str()), lst);
    else
        setMessage(MSG_TYPE_LIST, &s_list, lst);

    flash();
    update();
}

void UIDisplay::onAny(t_symbol* s, const AtomListView& lst)
{
    AutoGuard g(auto_);

    setMessage(Atom(s).isProperty() ? MSG_TYPE_PROPERTY : MSG_TYPE_ANY, s, lst);

    flash();
    update();
}

void UIDisplay::onProperty(t_symbol* s, const AtomListView& lst)
{
    if (s == SYM_PROP_SIZE && asEBox()->b_resize) {
        eclass_attr_setter(asPdObject(), SYM_SIZE, lst.size(), lst.toPdData());
        return;
    }

    AutoGuard g(auto_);
    setMessage(MSG_TYPE_PROPERTY, s, lst);
    flash();
    update();
}

void UIDisplay::onDblClick(t_object* /*view*/, const t_pt& /*pt*/, long /*modifiers*/)
{
    AutoGuard g(auto_);
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

const char* UIDisplay::annotateInlet(int /*n*/) const
{
    return "any message";
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
    redrawAll();
}

void UIDisplay::flash()
{
    if (prop_display_events) {
        on_bang_ = true;
        timer_.delay(100);
    }
}

void UIDisplay::m_resize(const AtomListView& lv)
{
    if (prop_auto_size) {
        auto_ = false;
        auto w = lv.floatAt(0, 0);
        auto h = lv.floatAt(1, 0);
        if (w > 0 && h > 0) {
            resize(w / zoom(), h / zoom());
        }
    }
}

void UIDisplay::redrawAll()
{
    bg_layer_.invalidate();
    redraw();
}

void UIDisplay::setup()
{
    sys_gui(ui_display_tcl);

    UIObjectFactory<UIDisplay> obj("ui.display");
    obj.addAlias("ui.d");
    obj.addAlias(SYM_UI_DT->s_name);
    obj.hideLabel();
    obj.useAnnotations();

    obj.setDefaultSize(80, 18);

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

    obj.addMethod(".resize", &UIDisplay::m_resize);
}

void setup_ui_display()
{
    SYM_DATA_TYPE = gensym("data");
    SYM_PROP_SIZE = gensym("@size");
    SYM_SIZE = gensym("size");
    SYM_DEFAULT = gensym("...");
    SYM_UI_DT = gensym("ui.dt");

    UIDisplay::setup();
}

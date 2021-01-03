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
#include "ui_label.h"
#include "ceammc_abstractdata.h"
#include "ceammc_convert.h"
#include "ceammc_format.h"
#include "ceammc_ui.h"

static t_symbol* SYM_LEFT;
static t_symbol* SYM_CENTER;
static t_symbol* SYM_RIGHT;
static t_symbol* SYM_TEXT;

#ifdef __WIN32
static const char* DEFAULT_LABEL_FONT_SIZE = "28";
#else
static const char* DEFAULT_LABEL_FONT_SIZE = "32";
#endif

UILabel::UILabel()
    : text_(&asEBox()->b_font, ColorRGBA::black(), ETEXT_UP_LEFT, ETEXT_JLEFT, ETEXT_WRAP)
    , prop_text_color(rgba_black)
    , prop_margin_top(5)
    , prop_margin_left(5)
    , prop_margin_bottom(5)
    , prop_margin_right(5)
    , prop_align(gensym("left"))
{
}

void UILabel::okSize(t_rect* newrect)
{
    newrect->width = pd_clip_min(newrect->width, 20);
    newrect->height = pd_clip_min(newrect->height, 20);
}

void UILabel::paint()
{
    const auto r = rect();
    UIPainter p = bg_layer_.painter(r);

    if (p) {
        text_.setColor(prop_text_color);

        float w = width() - (prop_margin_left + prop_margin_right);
        float h = height() - (prop_margin_top + prop_margin_bottom);

        const char* TXT = text_str_.c_str();

        if (prop_align == SYM_LEFT) {
            text_.setJustify(ETEXT_JLEFT);
            text_.setAnchor(ETEXT_UP_LEFT);
            text_.set(TXT, prop_margin_left, prop_margin_top, w, h);
        } else if (prop_align == SYM_CENTER) {
            text_.setJustify(ETEXT_JCENTER);
            text_.setAnchor(ETEXT_UP);
            text_.set(TXT, prop_margin_left + w / 2, prop_margin_top, w, h);
        } else if (prop_align == SYM_RIGHT) {
            text_.setJustify(ETEXT_JRIGHT);
            text_.setAnchor(ETEXT_UP_RIGHT);
            text_.set(TXT, width() - prop_margin_right, prop_margin_top, w, h);
        }

        p.drawText(text_);
    }
}

void UILabel::setDrawParams(t_edrawparams* params)
{
    UIObject::setDrawParams(params);
    // set border and background to the same color
    params->d_bordercolor = prop_color_background;
    params->d_boxfillcolor = prop_color_background;
}

void UILabel::init(t_symbol* name, const AtomList& args, bool usePresets)
{
    UIObject::init(name, args, usePresets);

    int pos = args.findPos(isProperty);
    if (pos > 0) {
        setProperty(SYM_TEXT, args.view(0, pos));
    } else if (pos < 0 && args.size() > 0) {
        setProperty(SYM_TEXT, args);
    }
}

void UILabel::onBang()
{
    // should be empty
}

void UILabel::onList(const AtomListView& lst)
{
    text_str_ = to_string(lst, " ");
    redrawBGLayer();
}

void UILabel::onAny(t_symbol* s, const AtomListView& lst)
{
    text_str_ = to_string(Atom(s) + lst, " ");
    redrawBGLayer();
}

void UILabel::onData(const Atom& data)
{
    text_str_ = data.asData()->toString();
    redrawBGLayer();
}

void UILabel::m_clear(const AtomListView&)
{
    text_str_ = "";
    redrawBGLayer();
}

void UILabel::m_append(const AtomListView& lst)
{
    std::string s = to_string(lst);
    if (s.empty())
        return;

    text_str_ += ' ';
    text_str_ += s;
    redrawBGLayer();
}

void UILabel::m_prepend(const AtomListView& lst)
{
    std::string s = to_string(lst);
    if (s.empty())
        return;

    text_str_ = s + " " + text_str_;
    redrawBGLayer();
}

void UILabel::setup()
{
    SYM_LEFT = gensym("left");
    SYM_CENTER = gensym("center");
    SYM_RIGHT = gensym("right");
    SYM_TEXT = gensym("text");

    UIObjectFactory<UILabel> obj("ui.label", EBOX_GROWINDI | EBOX_IGNORELOCKCLICK, CLASS_NOINLET);
    obj.setDefaultSize(300, 47);
    obj.hideLabel();

    obj.hideProperty("border_color");
    obj.hideProperty("send");
    obj.showProperty("fontname");
    obj.showProperty("fontsize");
    obj.showProperty("fontweight");
    obj.showProperty("fontslant");
    obj.setPropertyDefaultValue("fontsize", DEFAULT_LABEL_FONT_SIZE);
    obj.setPropertyDefaultValue("fontweight", "normal");
    obj.setPropertyDefaultValue("background_color", "1 1 1 1");
    obj.setPropertyDefaultValue("pinned", "1");
    // change default categories
    obj.setPropertyCategory("fontname", "Main");
    obj.setPropertyCategory("fontsize", "Main");
    obj.setPropertyCategory("fontweight", "Main");
    obj.setPropertyCategory("fontslant", "Main");

    obj.addProperty(PROP_TEXT_COLOR, _("Text Color"), DEFAULT_TEXT_COLOR, &UILabel::prop_text_color);

    obj.addProperty("margin_top", _("Margin top"), 5, &UILabel::prop_margin_top, "Margins");
    obj.addProperty("margin_left", _("Margin left"), 5, &UILabel::prop_margin_left, "Margins");
    obj.addProperty("margin_bottom", _("Margin bottom"), 5, &UILabel::prop_margin_bottom, "Margins");
    obj.addProperty("margin_right", _("Margin right"), 5, &UILabel::prop_margin_right, "Margins");

    obj.addProperty("align", _("Align"), SYM_LEFT->s_name, &UILabel::prop_align, "left center right", "Main");
    obj.addVirtualProperty("text", _("Text"), "Label", &UILabel::propGetText, &UILabel::propSetText);
    obj.setPropertyCategory("text", "Main");

    obj.useList();
    obj.useAny();
    obj.useBang();
    obj.useData();

    obj.addMethod("clear", &UILabel::m_clear);
    obj.addMethod("append", &UILabel::m_append);
    obj.addMethod("prepend", &UILabel::m_prepend);
}

AtomList UILabel::propGetText() const
{
    return prop_text;
}

void UILabel::propSetText(const AtomListView& lst)
{
    text_str_ = to_string(lst, " ");
    prop_text = lst;
}

const std::string& UILabel::text() const
{
    return text_str_;
}

void setup_ui_label()
{
    UILabel::setup();
}

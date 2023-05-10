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
#include "ceammc_canvas.h"
#include "ceammc_containers.h"
#include "ceammc_crc32.h"
#include "ceammc_format.h"
#include "ceammc_string.h"
#include "ceammc_ui.h"

CEAMMC_DEFINE_SYM_HASH(left)
CEAMMC_DEFINE_HASH(right)
CEAMMC_DEFINE_HASH(center)
CEAMMC_DEFINE_SYM_HASH(text)

extern t_symbol* ceammc_realizeraute(t_canvas* cnv, t_symbol* s);

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
    , prop_align(sym_left())
{
}

void UILabel::okSize(t_rect* newrect)
{
    newrect->w = pd_clip_min(newrect->w, 20);
    newrect->h = pd_clip_min(newrect->h, 20);
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

        auto prop_hash = crc32_hash(prop_align);

        if (prop_hash == hash_left) {
            text_.setJustify(ETEXT_JLEFT);
            text_.setAnchor(ETEXT_UP_LEFT);
            text_.set(TXT, prop_margin_left, prop_margin_top, w, h);
        } else if (prop_hash == hash_center) {
            text_.setJustify(ETEXT_JCENTER);
            text_.setAnchor(ETEXT_UP);
            text_.set(TXT, prop_margin_left + w / 2, prop_margin_top, w, h);
        } else if (prop_hash == hash_right) {
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

void UILabel::init(t_symbol* name, const AtomListView& args, bool usePresets)
{
    UIObject::init(name, args, usePresets);

    auto it = std::find_if(args.begin(), args.end(), isProperty);
    auto pos = std::distance(args.begin(), it);

    if (pos > 0) {
        setProperty(sym_text(), args.subView(0, pos));
    } else if (it == args.end() && args.size() > 0) {
        setProperty(sym_text(), args);
    }
}

void UILabel::onBang()
{
    // should be empty
}

void UILabel::onList(const AtomListView& lv)
{
    string::SmallString str;
    string::list_to_string(lv, str);
    text_str_.assign(str.data(), str.size());

    redrawBGLayer();
}

void UILabel::onAny(t_symbol* s, const AtomListView& lv)
{
    SmallAtomList lst { s };
    lst.insert(lst.end(), lv.begin(), lv.end());

    string::SmallString str;
    string::list_to_string(lst.view(), str);
    text_str_.assign(str.data(), str.size());

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

void UILabel::m_append(const AtomListView& lv)
{
    std::string s = to_string(lv);
    if (s.empty())
        return;

    text_str_ += ' ';
    text_str_ += s;
    updateDollarRaute();
    redrawBGLayer();
}

void UILabel::m_prepend(const AtomListView& lv)
{
    std::string s = to_string(lv);
    if (s.empty())
        return;

    text_str_ = s + " " + text_str_;
    updateDollarRaute();
    redrawBGLayer();
}

void UILabel::setup()
{
    UIObjectFactory<UILabel> obj("ui.label", EBOX_GROWINDI | EBOX_IGNORELOCKCLICK, CLASS_NOINLET);
    obj.setDefaultSize(300, 47);
    obj.hideLabel();

    obj.internalProperty("border_color");
    obj.internalProperty("send");
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

    obj.addProperty("align", _("Align"), str_left, &UILabel::prop_align, "left center right", "Main");
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

void UILabel::updateDollarRaute()
{
    if (text_str_.find('#') != std::string::npos) {
        for (auto& c : text_str_) {
            if (c == '#')
                c = '$';
        }

        text_str_ = canvas_realizedollar(canvas(), gensym(text_str_.c_str()))->s_name;
    }
}

AtomList UILabel::propGetText() const
{
    return prop_text;
}

void UILabel::propSetText(const AtomListView& lv)
{
    prop_text = lv;
    text_str_ = to_string(lv, " ");
    updateDollarRaute();
}

const std::string& UILabel::text() const
{
    return text_str_;
}

void setup_ui_label()
{
    UILabel::setup();
}

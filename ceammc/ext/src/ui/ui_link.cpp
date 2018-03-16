//
//  ui_link.cpp
//  pd_ext
//
//  Created by Alex Nadzharov on 22/12/16.
//
//

#include <cmath>
#include <stdio.h>

#include "ceammc_format.h"
#include "ceammc_ui.h"
#include "ui_link.h"
#include "ui_link_tcl.h"

//#include "m_imp.h" /* FIXME need access to c_externdir... */

#ifdef __WIN32
static t_symbol* LINK_FONT = gensym("Verdana");
static float FIX_LINK_Y_POS = 5;
static float FIX_TEXT_Y_OFF = 0;
#elif __APPLE__
static t_symbol* LINK_FONT = gensym("Menlo");
static float FIX_LINK_Y_POS = 3;
static float FIX_TEXT_Y_OFF = 1;
#else
static t_symbol* LINK_FONT = gensym("Terminus");
static float FIX_LINK_Y_POS = 3;
static float FIX_TEXT_Y_OFF = 0;
#endif

static size_t text_width(t_symbol* txt, int sz)
{
    const size_t len = strlen(txt->s_name);
    size_t char_wd = static_cast<size_t>(sys_fontwidth(sz));
    int corr = 0;

#ifdef __APPLE__
    if (len < 6)
        corr = 3;
    if (len > 16)
        corr = -3;
#endif

#ifdef __WIN32
    char_wd += 2;
#endif

    return char_wd * len + corr;
}

UILink::UILink()
    : prop_color_link(rgba_blue)
    , prop_color_hover(rgba_red)
    , prop_url(&s_)
    , prop_title(&s_)
    , canvas_dir_(&s_)
    , link_text_(&asEBox()->b_font, ColorRGBA::black(), ETEXT_LEFT, ETEXT_JLEFT, ETEXT_NOWRAP)
    , hover_(false)
{
}

void UILink::okSize(t_rect* newrect)
{
    float w = prop_title ? text_width(prop_title, FONT_SIZE) * zoom() : 40;
    newrect->width = pd_clip_min(w, 20);
    float h = ebox_fontheight(asEBox());

#ifndef __WIN32
    newrect->height = h;
#else
    newrect->height = floorf(1.5 * h) * zoom();
#endif
}

void UILink::paint(t_object* view)
{
    const t_rect& r = rect();
    UIPainter p = bg_layer_.painter(r);

    if (!p)
        return;

    asEBox()->b_font.c_family = LINK_FONT;
    asEBox()->b_font.c_size = FONT_SIZE;
    link_text_.setFont(&asEBox()->b_font);

    link_text_.setColor(hover_ ? prop_color_hover : prop_color_link);
    link_text_.set(prop_title->s_name, 3, r.height / 2, 0, 0);

    p.drawText(link_text_);
}

void UILink::setDrawParams(t_object*, t_edrawparams* params)
{
    params->d_borderthickness = 1;
    params->d_bordercolor = prop_color_background;
    params->d_boxfillcolor = prop_color_background;
    params->d_cornersize = 0;
}

t_pd_err UILink::notify(t_symbol* attr_name, t_symbol* msg)
{
    if (msg == s_attr_modified) {
        if (attr_name == gensym("title")) {
            size_t w = text_width(prop_title, FONT_SIZE) * zoom();
            resize(w, 0);
        }
    }

    return 0;
}

void UILink::onMouseDown(t_object* view, const t_pt& pt, long modifiers)
{
    t_canvas* c = reinterpret_cast<t_canvas*>(view);
    if (!c) {
        UI_ERR << "invalid canvas";
        return;
    }

    t_symbol* dir = canvas_getdir(c);
    sys_vgui("ceammclink_open {%s} {%s}\n", prop_url->s_name, dir->s_name);
}

void UILink::onMouseEnter(t_object* view, const t_pt& pt, long modifiers)
{
    hover_ = true;
    redrawBGLayer();
}

void UILink::onMouseLeave(t_object* view, const t_pt& pt, long modifiers)
{
    hover_ = false;
    redrawBGLayer();
}

AtomList UILink::p_title() const
{
    return AtomList(prop_title);
}

void UILink::p_setTitle(const AtomList& lst)
{
    prop_title = gensym(to_string(lst).c_str());
}

void UILink::setup()
{
    UIObjectFactory<UILink> obj("ui.link", EBOX_GROWNO, CLASS_NOINLET);
    obj.hideProperty("send");
    obj.hideProperty("receive");
    obj.hideProperty("size");
    obj.hideProperty(PROP_BACKGROUND_COLOR);
    obj.hideProperty(PROP_BORDER_COLOR);

    obj.setDefaultSize(120, 15);
    obj.setPropertyDefaultValue(PROP_BACKGROUND_COLOR, "1.0 1.0 1.0 1.0");

    obj.addSymbolProperty("url", _("URL"), "<no url>", &UILink::prop_url);
    obj.addSymbolProperty("title", _("Title"), "<no title>", &UILink::prop_title);
    obj.setPropertyAccessor("title", &UILink::p_title, &UILink::p_setTitle);
    obj.addProperty("link_color", _("Link Color"), "0 0.625 0.75 1", &UILink::prop_color_link);
    obj.addProperty("hover_color", _("Hover Color"), "1 0 0.5 1", &UILink::prop_color_hover);

    obj.useMouseEvents(UI_MOUSE_LEAVE | UI_MOUSE_ENTER | UI_MOUSE_DOWN);
}

void setup_ui_link()
{
    uilink_tcl_init();
    UILink::setup();
}

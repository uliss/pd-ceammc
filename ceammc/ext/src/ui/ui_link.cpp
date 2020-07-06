//
//  ui_link.cpp
//  pd_ext
//
//  Created by Alex Nadzharov on 22/12/16.
//
//

#include <cinttypes>
#include <cmath>
#include <cstdio>

#include "ceammc_format.h"
#include "ceammc_ui.h"
#include "ui_link.h"
#include "ui_link_tcl.h"

static t_symbol* LINK_FONT;
constexpr int FIX_LINK_X_OFF = 3;
#ifdef __WIN32
static float FIX_LINK_Y_POS = 5;
static float FIX_TEXT_Y_OFF = 0;
#elif __APPLE__
static float FIX_LINK_Y_POS = 3;
static float FIX_TEXT_Y_OFF = 2;
#else
static float FIX_LINK_Y_POS = 3;
static float FIX_TEXT_Y_OFF = 0;
#endif

static size_t text_width(t_symbol* txt, int sz)
{
    const size_t len = strlen(txt->s_name);
    float char_wd = static_cast<size_t>(sys_fontwidth(sz));
    int corr = 0;

#ifdef __APPLE__
    if (len < 6) {
        char_wd += 0.5;
        corr = 3;
    } else {
        char_wd += 0.6;
        corr = 2;
    }
#elif __WIN32
    char_wd += 3;
#else
    char_wd += 3;
#endif

    return std::round(char_wd * len) + corr;
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
    newrect->width = pd_clip_min(newrect->width, FONT_SIZE);
    float h = ebox_fontheight(asEBox());

#ifdef __APPLE__
    newrect->height = std::round(h * 1.3);
#else
    newrect->height = floorf(1.5 * h);
#endif
}

void UILink::paint()
{
    const auto r = rect();
    UIPainter p = bg_layer_.painter(r);

    if (!p)
        return;

    asEBox()->b_font.c_family = LINK_FONT;
    asEBox()->b_font.c_size = FONT_SIZE;
    link_text_.setFont(&asEBox()->b_font);

    link_text_.setColor(hover_ ? prop_color_hover : prop_color_link);
    link_text_.set(prop_title->s_name, FIX_LINK_X_OFF, r.height / 2 + FIX_TEXT_Y_OFF, 0, 0);

    p.drawText(link_text_);
}

void UILink::setDrawParams(t_edrawparams* params)
{
    UIObject::setDrawParams(params);
    params->d_bordercolor = prop_color_background;
    params->d_boxfillcolor = prop_color_background;
}

void UILink::onPropChange(t_symbol* prop_name)
{
    if (prop_name == gensym("title")) {
        char buf[MAXPDSTRING] = { 0 };
        sprintf(buf, "#ui.link%" PRIxPTR, reinterpret_cast<uintptr_t>(this));
        sys_vgui("pdsend \"%s @size [expr %d + [font measure UILinkFont {%s}]] 16\"\n",
            buf, FIX_LINK_X_OFF * 2, prop_title->s_name);
    }
}

void UILink::onMouseDown(t_object* view, const t_pt& pt, const t_pt& abs_pt, long modifiers)
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
    UIObjectFactory<UILink> obj("ui.link", /*EBOX_GROWNO*/ EBOX_GROWINDI, CLASS_NOINLET);
    obj.hideProperty("send");
    obj.hideProperty("receive");
    obj.hideProperty("size");
    obj.hideLabel();
    obj.hideProperty(PROP_BACKGROUND_COLOR);
    obj.hideProperty(PROP_BORDER_COLOR);
    obj.setPropertySave("@size", false);

    obj.setDefaultSize(120, 15);
    obj.setPropertyDefaultValue(PROP_BACKGROUND_COLOR, "1.0 1.0 1.0 1.0");

    obj.addSymbolProperty("url", _("URL"), "<no url>", &UILink::prop_url, _("Main"));
    obj.addSymbolProperty("title", _("Title"), "<no title>", &UILink::prop_title, _("Main"));
    obj.setPropertyAccessor("title", &UILink::p_title, &UILink::p_setTitle);
    obj.addProperty("link_color", _("Link Color"), "0 0.625 0.75 1", &UILink::prop_color_link);
    obj.addProperty("hover_color", _("Hover Color"), "1 0 0.5 1", &UILink::prop_color_hover);

    obj.useMouseEvents(UI_MOUSE_LEAVE | UI_MOUSE_ENTER | UI_MOUSE_DOWN);
}

void setup_ui_link()
{
#ifdef __WIN32
    LINK_FONT = gensym("DejaVu Sans Mono");
#elif __APPLE__
    LINK_FONT = gensym("Menlo");
#else
    LINK_FONT = gensym("DejaVu Sans Mono");
#endif

    uilink_tcl_init();
    sys_vgui("font create UILinkFont -family {%s} -size %d\n", LINK_FONT->s_name, UILink::FONT_SIZE);
    UILink::setup();
}

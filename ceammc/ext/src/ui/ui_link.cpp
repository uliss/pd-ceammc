//
//  ui_link.cpp
//  pd_ext
//
//  Created by Alex Nadzharov on 22/12/16.
//
//

#include "ui_link.h"
#include "ceammc_ui.h"
#include "ui_link.tcl.h"

UILink::UILink()
    : prop_color_link(rgba_blue)
    , prop_color_hover(rgba_red)
    , prop_url(&s_)
    , prop_title(&s_)
    , canvas_dir_(&s_)
{
    char buf[64];
    snprintf(buf, 64, "r%p", this);
    rid_ = gensym(buf);
    pd_bind(asPd(), rid_);

    if (canvas())
        canvas_dir_ = canvas_getdir(canvas());
}

UILink::~UILink()
{
    pd_unbind(asPd(), rid_);
}

void UILink::init(t_symbol* name, const AtomListView& args, bool usePresets)
{
    UIObject::init(name, args, usePresets);

    if (args.size() >= 2 && args[0].isSymbol()) {
        prop_url = args[0].asT<t_symbol*>();
        p_setTitle(args.subView(1));
    } else if (args.size() == 1 && args[0].isSymbol()) {
        prop_url = args[0].asT<t_symbol*>();
        prop_title = prop_url;
    }
}

void UILink::paint()
{
    sys_vgui("ui::link_update %s %lx %s %d %d %d "
             "#%6.6x #%6.6x {%s} {%s}\n",
        asEBox()->b_canvas_id->s_name, asEBox(), rid_->s_name,
        (int)width(), (int)height(), (int)zoom(),
        rgba_to_hex_int(prop_color_link), rgba_to_hex_int(prop_color_hover),
        prop_url->s_name,
        prop_title->s_name);
}

void UILink::m_resize(const AtomListView& lv)
{
    auto w = lv.floatAt(0, 0);
    auto h = lv.floatAt(1, 0);
    if (w > 0 && h > 0)
        resize(w / zoom(), h / zoom());
}

void UILink::onMouseDown(t_object* /*view*/, const t_pt& /*pt*/, const t_pt& /*abs_pt*/, long /*modifiers*/)
{
    sys_vgui("ui::link_open {%s} {%s}\n", prop_url->s_name, canvas_dir_->s_name);
}

AtomList UILink::p_title() const
{
    return AtomList(prop_title);
}

void UILink::setDrawParams(t_edrawparams* params)
{
    UIObject::setDrawParams(params);
    params->d_hideborder = true;
}

void UILink::p_setTitle(const AtomListView& lv)
{
    prop_title = gensym(to_string(lv).c_str());
}

void UILink::setup()
{
    UIObjectFactory<UILink> obj("ui.link", EBOX_GROWNO, CLASS_NOINLET);
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

    obj.useMouseEvents(UI_MOUSE_DOWN);
    obj.addMethod(".resize", &UILink::m_resize);
}

void setup_ui_link()
{
    sys_gui(ui_link_tcl);
    UILink::setup();
}

/*****************************************************************************
 * Copyright 2021 Serge Poltavsky. All rights reserved.
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
#include "ui_button.h"
#include "ceammc_ui.h"
#include "ui_button.tcl.h"

constexpr int MIN_SIZE = 8;

UIButton::UIButton()
    : prop_color_active(rgba_blue)
    , active_(false)
{
    createOutlet();
}

void UIButton::okSize(t_rect* newrect)
{
    newrect->width = pd_clip_min(newrect->width, MIN_SIZE);
    newrect->height = pd_clip_min(newrect->height, MIN_SIZE);
}

void UIButton::paint()
{
    sys_vgui("ui::button_delete #%x %s\n",
        asEBox(), asEBox()->b_drawing_id->s_name);

    sys_vgui("ui::button_create #%x %s "
             "%d %d "
             "#%6.6x #%6.6x "
             "%d\n",
        asEBox(), asEBox()->b_drawing_id->s_name,
        (int)width(), (int)height(),
        rgba_to_hex_int(prop_color_border), rgba_to_hex_int(prop_color_active),
        active_ ? 1 : 0);
}

void UIButton::onFloat(t_float f)
{
    if (f != 0)
        on();
    else
        off();
}

void UIButton::onMouseDown(t_object* view, const t_pt& pt, const t_pt& abs_pt, long modifiers)
{
    on();
}

void UIButton::onMouseUp(t_object* view, const t_pt& pt, long modifiers)
{
    off();
}

void UIButton::on()
{
    if (active_)
        return;

    active_ = true;
    floatTo(0, prop_value_on);
    send(prop_value_on);

    bg_layer_.invalidate();
    redrawInnerArea();
}

void UIButton::off()
{
    if (!active_)
        return;

    active_ = false;
    floatTo(0, prop_value_off);
    send(prop_value_off);

    bg_layer_.invalidate();
    redrawInnerArea();
}

void UIButton::setup()
{
    sys_gui(ui_button_tcl);

    UIObjectFactory<UIButton> obj("ui.button", EBOX_GROWLINK);
    obj.addAlias("ui.btn");
    obj.useFloat();

    obj.setDefaultSize(15, 15);
    obj.addProperty(PROP_ACTIVE_COLOR, _("Active Color"), DEFAULT_ACTIVE_COLOR, &UIButton::prop_color_active);
    obj.addFloatProperty("on_value", _("On value"), 1, &UIButton::prop_value_on, _("Main"));
    obj.addFloatProperty("off_value", _("Off value"), 0, &UIButton::prop_value_off, _("Main"));

    obj.useMouseEvents(UI_MOUSE_DOWN | UI_MOUSE_UP);
}

void setup_ui_button()
{
    UIButton::setup();
}

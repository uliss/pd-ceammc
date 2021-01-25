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
#include "ui_bang.h"
#include "ceammc_ui.h"
#include "ui_bang.tcl.h"

UIBang::UIBang()
    : clock_(this, &UIBang::deactivate)
    , active_(false)
    , prop_color_active(rgba_blue)
{
    createOutlet();
}

void UIBang::okSize(t_rect* newrect)
{
    newrect->width = pd_clip_min(newrect->width, 8);
    newrect->height = pd_clip_min(newrect->height, 8);
}

void UIBang::paint()
{
    sys_vgui("::ui::bang_update %s %lx %d %d %d #%6.6x %d\n",
        asEBox()->b_canvas_id->s_name, asEBox(),
        (int)width(), (int)height(), (int)zoom(),
        active_ ? rgba_to_hex_int(prop_color_active)
                : rgba_to_hex_int(prop_color_border),
        (int)active_);
}

void UIBang::onMouseDown(t_object* view, const t_pt& pt, const t_pt& abs_pt, long modifiers)
{
    activate();
}

void UIBang::onMouseUp(t_object* view, const t_pt& pt, long modifiers)
{
    deactivate();
}

void UIBang::onAny(t_symbol* s, const AtomListView&)
{
    activate();
    clock_.delay(100);
}

void UIBang::activate()
{
    active_ = true;
    bg_layer_.invalidate();
    redraw();
    bangTo(0);
    sendBang();
}

void UIBang::deactivate()
{
    active_ = false;
    bg_layer_.invalidate();
    redraw();
}

void UIBang::setup()
{
    sys_gui(ui_bang_tcl);

    UIObjectFactory<UIBang> obj("ui.bang", EBOX_GROWLINK);
    obj.addAlias("ui.b");

    obj.setDefaultSize(15, 15);
    obj.addProperty(PROP_ACTIVE_COLOR, _("Active Color"), DEFAULT_ACTIVE_COLOR, &UIBang::prop_color_active);

    obj.useMouseEvents(UI_MOUSE_DOWN | UI_MOUSE_UP);
    obj.useAny();
}

void setup_ui_bang()
{
    UIBang::setup();
}

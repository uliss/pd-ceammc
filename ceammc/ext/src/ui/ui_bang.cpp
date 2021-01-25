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

    // We defines that the width and the height can't be an even number (to center the bang circle).
    if ((int)newrect->width % 2 == 1)
        newrect->width++;
    if ((int)newrect->height % 2 == 1)
        newrect->height++;
}

void UIBang::paint()
{
    const auto r = rect();
    UIPainter p = bg_layer_.painter(r);

    if (!p)
        return;

    const int box_center = static_cast<int>(floorf(r.width * 0.5f));
    int circle_radius = static_cast<int>(roundf(box_center * 0.9f));
    if (circle_radius >= box_center)
        circle_radius--;

    // fix for small sizes
    if (r.width < 20)
        circle_radius = box_center;

    // align fix
    if ((box_center - circle_radius) % 2 == 1)
        circle_radius--;

    p.drawCircle(box_center, box_center, circle_radius);
    if (active_) {
        p.setColor(prop_color_active);
        p.fill();
    } else {
        p.setColor(prop_color_border);
        p.stroke();
    }
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
    UIObjectFactory<UIBang> obj("ui.bang", EBOX_GROWLINK);
    obj.addAlias("ui.b");

    obj.setDefaultSize(16, 16);
    obj.addProperty(PROP_ACTIVE_COLOR, _("Active Color"), DEFAULT_ACTIVE_COLOR, &UIBang::prop_color_active);

    obj.useMouseEvents(UI_MOUSE_DOWN | UI_MOUSE_UP);
    obj.useAny();
}

void setup_ui_bang()
{
    UIBang::setup();
}

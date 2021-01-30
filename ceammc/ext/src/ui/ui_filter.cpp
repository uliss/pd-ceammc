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
#include "ui_filter.h"
#include "ceammc_ui.h"
#include "ui_filter.tcl.h"

UIFilter::UIFilter()
    : b0_(1)
    , b1_(0)
    , b2_(0)
    , a1_(0)
    , a2_(0)
{
}

bool UIFilter::okSize(t_rect* newrect)
{
    newrect->width = pd_clip_min(newrect->width, 100);
    newrect->height = pd_clip_min(newrect->height, 60);
    return true;
}

void UIFilter::paint()
{
    sys_vgui("ui::filter_update %s %lx %d %d %d "
             "#%6.6x "
             "%f %f %f %f %f \n",
        asEBox()->b_canvas_id->s_name, asEBox(), (int)width(), (int)height(), (int)zoom(),
        rgba_to_hex_int(prop_color_border),
        b0_, b1_, b2_, a1_, a2_);
}

void UIFilter::onList(const AtomListView& lv)
{
    if (lv.size() != 5) {
        return;
    }

    b0_ = lv[0].asFloat();
    b1_ = lv[1].asFloat();
    b2_ = lv[2].asFloat();
    a1_ = lv[3].asFloat();
    a2_ = lv[4].asFloat();

    redraw();
}

void UIFilter::setup()
{
    sys_gui(ui_filter_tcl);

    UIObjectFactory<UIFilter> obj("ui.filter", EBOX_GROWINDI);
    obj.hideLabelInner();

    obj.useList();
    //    obj.useAny();
    //    obj.usePopup();
    obj.setDefaultSize(300, 100);
    obj.useMouseEvents(UI_MOUSE_DOWN);
}

void setup_ui_filter()
{
    UIFilter::setup();
}

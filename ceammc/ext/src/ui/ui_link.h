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
#ifndef UI_LINK_H
#define UI_LINK_H

#include "ceammc_ui_object.h"
using namespace ceammc;

class UILink : public UIObject {
    t_rgba prop_color_link;
    t_rgba prop_color_hover;
    t_symbol* prop_url;
    t_symbol* prop_title;
    t_symbol* canvas_dir_;
    UITextLayout link_text_;
    bool hover_;

public:
    UILink();

    void okSize(t_rect* newrect);
    void paint();
    void setDrawParams(t_edrawparams* params);
    void onPropChange(t_symbol* prop_name);

    void onMouseDown(t_object* view, const t_pt& pt, const t_pt& abs_pt, long modifiers);
    void onMouseEnter(t_object* view, const t_pt& pt, long modifiers);
    void onMouseLeave(t_object* view, const t_pt& pt, long modifiers);

    AtomList p_title() const;
    void p_setTitle(const AtomListView& lst);

public:
    static void setup();
};

void setup_ui_link();

#endif // UI_LINK_H

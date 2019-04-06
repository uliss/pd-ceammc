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
#ifndef UI_DISPLAY_H
#define UI_DISPLAY_H

#include "ceammc_clock.h"
#include "ceammc_ui_object.h"

using namespace ceammc;

class UIDisplay : public UIObject {
    int prop_display_events;
    int prop_display_type;
    int prop_auto_size;
    bool on_bang_;
    int type_width_;
    t_rgba prop_text_color;
    t_rgba prop_active_color;

private:
    UIFont font_;
    UITextLayout txt_value_;
    UITextLayout txt_type_;
    std::string msg_txt_;
    t_symbol* msg_type_;
    ClockMemberFunction<UIDisplay> timer_;
    double last_update_;

public:
    UIDisplay();

    void paint(t_canvas*);
    void okSize(t_rect* newrect);

    void onBang();
    void onFloat(t_float f);
    void onSymbol(t_symbol* s);
    void onList(const AtomList& lst);
    void onAny(t_symbol* s, const AtomList& lst);
    void onProperty(t_symbol* s, const AtomList& lst);

    void onDblClick(t_object* view, const t_pt& pt, long modifiers);

    const std::string& text() const;
    const std::string type() const;

public:
    static void setup();

    void onClock();
    void update();
    void flash();

private:
    void redrawAll();
};

void setup_ui_display();

#endif // UI_DISPLAY_H

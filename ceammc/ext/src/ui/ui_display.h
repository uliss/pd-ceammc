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

enum UIMessageType : char {
    MSG_TYPE_BANG = 0,
    MSG_TYPE_FLOAT,
    MSG_TYPE_SYMBOL,
    MSG_TYPE_PROPERTY,
    MSG_TYPE_LIST,
    MSG_TYPE_DATA,
    MSG_TYPE_ANY
};

class UIDisplay : public UIObject {
    int prop_display_events;
    int prop_display_type;
    int prop_auto_size;
    int prop_float_precision_;
    int prop_hex_format_width_;
    int prop_int_hex_;
    t_rgba prop_text_color;
    t_rgba prop_active_color;

private:
    ClockMemberFunction<UIDisplay> timer_;
    std::string msg_txt_;
    double last_update_;
    t_symbol* msg_type_;
    t_symbol* rid_;
    bool on_bang_;
    bool auto_;
    UIMessageType type_;

public:
    UIDisplay();
    ~UIDisplay();

    void paint();
    void paint(const char* txt);
    void okSize(::t_rect* newrect);
    void init(t_symbol* name, const AtomListView& args, bool usePresets);

    void onBang();
    void onFloat(t_float f);
    void onSymbol(t_symbol* s);
    void onList(const AtomListView& lv);
    void onAny(t_symbol* s, const AtomListView& lv);
    void onProperty(t_symbol* s, const AtomListView& lv);

    void onMouseDown(t_object* view, const t_pt& pt, const t_pt& abs_pt, long modifiers);
    void onDblClick(t_object* view, const t_pt& pt, long modifiers);

    const std::string& text() const;
    const std::string type() const;

    const char* annotateInlet(int n) const;

public:
    static void setup();

    void onClock();
    void update();
    void flash();

    void m_resize(const AtomListView& lv);

private:
    void copyToClipboard(bool whole);
    void redrawAll();
    void appendFloatToText(t_float f);
    void setMessage(UIMessageType t, t_symbol* s, const AtomListView& lv);
};

void setup_ui_display();

#endif // UI_DISPLAY_H

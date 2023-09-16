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
#ifndef UI_NUMBER_H
#define UI_NUMBER_H

#include "ceammc_clock.h"
#include "ceammc_ui_object.h"

using namespace ceammc;

class UINumber : public UIObject {
public:
    enum EditMode {
        MODE_DISPLAY = 0,
        MODE_WAIT_INPUT,
        MODE_DO_INPUT
    };

public:
    UINumber();

    void okSize(t_rect* newrect);
    void onPropChange(t_symbol* prop_name);
    void paint();
    void drawBackground();
    void drawValue();

    void onBang();
    void onFloat(t_float f);
    void setValue(t_float f);
    void onKey(int k, long modifiers);
    void onKeyFilter(int k, long modifiers);
    void onMouseDown(t_object* view, const t_pt& pt, const t_pt& abs_pt, long modifiers);
    void onMouseDrag(t_object* view, const t_pt& pt, long modifiers);
    void onMouseLeave(t_object* view, const t_pt& pt, long modifiers);
    void onDblClick(t_object* view, const t_pt& pt, long modifiers);

    // presets
    void loadPreset(size_t idx);
    void storePreset(size_t idx);
    void interpPreset(t_float idx);
    bool hasPresetInterp() const { return true; }

    // methods
    void m_plus(t_float f);
    void m_minus(t_float f);
    void m_mul(t_float f);
    void m_div(t_float f);
    void m_increment();
    void m_decrement();

    // members
    t_float value() const { return value_; }

public:
    static void setup();

private:
    void output();
    void redrawValue();
    void updateTextValue();

private:
    ClockMemberFunction<UINumber> clock_;
    UIFont font_;
    UITextLayout text_;
    UILayer text_layer_;
    std::string enter_value_;
    t_float value_;
    char text_value_[48];
    EditMode edit_mode_;
    double last_update_time_;
    float drag_start_ypos_;
    float drag_increment_;
    t_float drag_start_value_;

private:
    t_rgba prop_color_text;
    t_rgba prop_color_active;
    t_float prop_min;
    t_float prop_max;
    int prop_digits;
};

void setup_ui_number();

#endif // UI_NUMBER_H

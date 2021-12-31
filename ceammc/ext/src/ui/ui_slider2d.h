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
#ifndef UI_SLIDER2D_H
#define UI_SLIDER2D_H

#include "ceammc_ui_object.h"

#include <cmath>

using namespace ceammc;

class UISlider2D : public UIObject {
    t_float prop_x_left;
    t_float prop_x_right;
    t_float prop_y_top;
    t_float prop_y_bottom;
    int prop_show_range;
    int prop_show_grid;

private:
    t_float x_pos_;
    t_float y_pos_;
    bool mouse_down_;

public:
    UISlider2D();

    void okSize(t_rect* newrect);
    void paint();
    void onPropChange(t_symbol* prop_name);

    void onBang();
    void onList(const AtomListView& lv);
    void onMouseDown(t_object* view, const t_pt& pt, const t_pt& abs_pt, long modifiers);
    void onMouseDrag(t_object* view, const t_pt& pt, long modifiers);
    void onMouseUp(t_object* view, const t_pt& pt, long modifiers);
    void onMouseWheel(const t_pt& pt, long modifiers, double delta);

    void m_set(const AtomListView& lv);
    void m_move(const AtomListView& lv);
    void loadPreset(size_t idx);
    void storePreset(size_t idx);
    void interpPreset(t_float idx);
    bool hasPresetInterp() const { return true; }

    bool setRealValue(const AtomListView& lv);
    AtomList realValue() const;
    t_float realXValue() const;
    t_float realYValue() const;

    t_float xRange() const { return std::abs(prop_x_right - prop_x_left); }
    t_float yRange() const { return std::abs(prop_y_bottom - prop_y_top); }

    void output();

    t_float propXRange() const;
    t_float propYRange() const;

    t_float propXValue() const;
    t_float propYValue() const;
    void propSetXValue(t_float x);
    void propSetYValue(t_float y);

public:
    static void setup();

    void setMouse(t_float x, t_float y);

private:
    void redrawKnob();
    void redrawAll();
};

void setup_ui_slider2d();

#endif // UI_SLIDER2D_H

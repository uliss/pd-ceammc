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
#ifndef UI_POLAR_H
#define UI_POLAR_H

#include "ceammc_ui_object.h"

using namespace ceammc;

class UIPolar : public UIObject {
    UIFont txt_font;
    UITextLayout txt_radius_;
    UITextLayout txt_angle_;
    UILayer knob_layer_;
    float radius_;
    float angle_;
    t_symbol* prop_direction_;
    bool mouse_down_;
    bool right_click_;
    int prop_clockwise_;
    int prop_radians_;
    int prop_positive_;

public:
    UIPolar();

    void okSize(t_rect* newrect);
    void paint();
    void onPropChange(t_symbol* prop_name);
    void paintBackground();
    void paintKnob();

    void onBang();
    void onList(const AtomList& lst);
    void onMouseDown(t_object* view, const t_pt& pt, const t_pt& abs_pt, long modifiers);
    void onMouseDrag(t_object* view, const t_pt& pt, long modifiers);
    void onMouseUp(t_object* view, const t_pt& pt, long modifiers);
    void onPopup(t_symbol* menu_name, long item_idx);
    void onMouseWheel(const t_pt& pt, long modifiers, double delta);

    void m_set(const AtomList& lst);
    void m_polar(const AtomList& lst);
    void m_cartesian(const AtomList& lst);
    void m_rotate(t_float angle);
    void loadPreset(size_t idx);
    void storePreset(size_t idx);

    bool setRealValue(const AtomList& lst);
    AtomList realValue() const;
    t_float realAngle() const;
    t_float realRadius() const;

    void output();

    AtomList propRadius() const;
    AtomList propAngle() const;
    void propSetRadius(const AtomList& lst);
    void propSetAngle(const AtomList& lst);

public:
    static void setup();

    void setMouse(float x, float y, bool angleOnly = false);

private:
    void redrawKnob();
    void redrawAll();
    double directionAngleOffset() const;
};

void setup_ui_polar();

#endif // UI_POLAR_H

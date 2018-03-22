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
#ifndef UI_RSLIDER_H
#define UI_RSLIDER_H

#include "ceammc_ui_object.h"

using namespace ceammc;

class UIRSlider : public UIObject {
    enum DragMode {
        NONE = 0,
        LOW,
        HIGH
    };

private:
    t_rgba prop_color_knob;
    float prop_min, prop_max;
    int prop_mouse_sync;

private:
    UILayer knob_layer_;
    float vlow_, vhigh_;
    bool is_horizontal_;
    DragMode drag_mode_;
    t_pt click_pt_;

public:
    UIRSlider();

    void init(t_symbol* name, const AtomList& args, bool usePresets);
    t_pd_err notify(t_symbol* attr_name, t_symbol* msg);
    void okSize(t_rect* newrect);
    void paint(t_object* view);
    void drawBackground();
    void drawKnob();

    void onBang();
    void onList(const AtomList& lst);

    void onMouseDown(t_object* view, const t_pt& pt, long modifiers);
    void onMouseUp(t_object* view, const t_pt& pt, long modifiers);
    void onMouseDrag(t_object* view, const t_pt& pt, long modifiers);

    AtomList propValue() const;
    void propSetValue(const AtomList& lst);
    t_float propLow() const;
    t_float propHigh() const;
    void propSetLow(t_float f);
    void propSetHigh(t_float f);

    t_float propRange() const;

    void loadPreset(size_t idx);
    void storePreset(size_t idx);

private:
    void adjustValues();
    void redrawKnob();
    void output();
    bool setValue(const AtomList& lst);

public:
    static void setup();
};

void setup_ui_rslider();

#endif // UI_RSLIDER_H

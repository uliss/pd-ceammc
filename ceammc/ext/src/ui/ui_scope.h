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
#ifndef UI_SCOPE_H
#define UI_SCOPE_H

#include "ceammc_clock.h"
#include "ceammc_ui_object.h"

using namespace ceammc;

class UIScope : public UIDspObject {
private:
    std::vector<float> data_;
    ClockMemberFunction<UIScope> redraw_timer_;
    UIFont txt_font_;
    UITextLayout txt0_;
    UITextLayout txt1_;
    UITextLayout txt2_;
    UITextLayout txt3_;
    UITextLayout txt4_;
    UILayer scope_layer_;
    bool freeze_;
    double last_redraw_time_;

    size_t window_phase_;
    size_t window_size_;

private:
    t_rgba prop_color_active;
    t_float prop_min;
    t_float prop_max;
    int prop_window;
    int prop_refresh;

public:
    UIScope();

    void okSize(t_rect* newrect);
    void onPropChange(t_symbol* prop_name);
    void paint();
    void paintBackground();
    void paintScope();

    void dspOn(double sr, long bs);
    void dspProcess(t_sample** ins, long, t_sample**, long, long sampleframes);

    void m_freeze(t_float f);
    void m_scale(t_float f);

    void onMouseDown(t_object* view, const t_pt& pt, const t_pt& abs_pt, long modifiers);
    void onDblClick(t_object* view, const t_pt& pt, long modifiers);
    void onMouseWheel(const t_pt& pt, long modifiers, float delta);

public:
    static void setup();

private:
    void redrawTick();
    void updateLabels();
    void updateLabel(UITextLayout& txt, float k);
    void calcDspVars();
};

void setup_ui_scope();

#endif // UI_SCOPE_H

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
#ifndef UI_GAIN_H
#define UI_GAIN_H

#include "ceammc_control.h"
#include "ceammc_object.h"
#include "ceammc_ui_dsp_object.h"

using namespace ceammc;

class UIGain : public UIDspObject {
private:
    UIFont font_;
    UITextLayout txt_max_;
    UITextLayout txt_min_;
    float knob_pos_;
    bool is_horizontal_;
    float rel_mode_delta_;

protected:
    SmoothControlValue smooth_;
    t_rgba prop_color_knob;
    t_rgba prop_color_scale;
    int prop_max;
    int prop_min;
    int prop_output_value;
    int prop_show_range;
    int prop_relative_mode;

    void initHorizontal();

public:
    UIGain();

    void okSize(t_rect* newrect);
    void paint();

    void init(t_symbol* name, const AtomList& args, bool usePresets);
    void dspProcess(t_sample** ins, long n_ins, t_sample** outs, long n_outs, long sampleframes);
    void onPropChange(t_symbol* prop_name);

    void onBang();
    void onMouseDown(t_object* view, const t_pt& pt, const t_pt& abs_pt, long modifiers);
    void onMouseDrag(t_object* view, const t_pt& pt, long modifiers);
    void onDblClick(t_object* view, const t_pt& pt, long modifiers);

    t_float dbValue() const;
    t_float ampValue() const;
    void setDbValue(t_float db);
    void setAmpValue(t_float amp);

    void loadPreset(size_t idx);
    void storePreset(size_t idx);

    void m_plus(t_float db);
    void m_minus(t_float db);
    void m_inc();
    void m_dec();

public:
    static void setup();

private:
    void updateLabels();
};

void setup_ui_gain();

#endif // UI_GAIN_H

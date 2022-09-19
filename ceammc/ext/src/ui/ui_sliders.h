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
#ifndef UI_SLIDERS_H
#define UI_SLIDERS_H

#include "ceammc_ui_object.h"

using namespace ceammc;

class UISliders : public UIObject {
    long select_idx_;
    bool is_vertical_;
    std::vector<t_float> pos_values_;
    std::vector<t_float> prev_pos_values_;

private:
    t_rgba prop_slider_color;
    t_rgba prop_select_color;
    t_float prop_min;
    t_float prop_max;
    t_symbol* prop_auto_range_mode;
    int prop_auto_count;
    int prop_show_range;
    int prop_show_lines;
    int prop_count;
    char c_min[16];
    char c_max[16];
    t_float click_phase_;

public:
    UISliders();

    void init(t_symbol* name, const AtomListView& args, bool usePresets);

    void okSize(t_rect* newrect);
    void onPropChange(t_symbol* prop_name);
    void paint();
    void paintSliders();

    void onBang();
    void onList(const AtomListView& lv);
    AtomList realValues() const;

    // presets
    void loadPreset(size_t idx);
    void storePreset(size_t idx);
    void interpPreset(t_float idx);
    bool hasPresetInterp() const { return true; }

    // mouse
    void onMouseDown(t_object* view, const t_pt& pt, const t_pt& abs_pt, long modifiers);
    void onMouseUp(t_object* view, const t_pt& pt, long modifiers);
    void onMouseDrag(t_object* view, const t_pt& pt, long modifiers);
    void onDblClick(t_object* view, const t_pt& pt, long modifiers);

    // methods
    void m_get(const AtomListView& lv);
    void m_set(const AtomListView& lv);
    void m_select(const AtomListView& lv);
    void m_plus(t_float f);
    void m_minus(t_float f);
    void m_mul(t_float f);
    void m_div(t_float f);
    void m_increment();
    void m_decrement();
    void m_fill(t_float v);
    void m_random();
    void m_linup();
    void m_lindown();

    void outputList();
    void normalize();
    void normalizeMin();
    void normalizeMax();

private:
    void setRealValueAt(size_t n, t_float v);
    t_float realValueAt(size_t n) const;
    bool setRealValues(const AtomListView& lv);

    t_float propCount() const;
    void setPropCount(t_float f);
    AtomList propAutoRangeMode() const;
    void setPropAutoRangeMode(const AtomListView& lv);

    t_float propRange() const;
    AtomList propValue() const;

    void generateTxtLabels();

    std::pair<t_float*, t_float> sliderPosValueAt(const t_pt& pt);

public:
    static void setup();

private:
    void redrawAll();
};

void setup_ui_sliders();

#endif // UI_SLIDERS_H

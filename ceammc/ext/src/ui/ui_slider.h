#ifndef UI_SLIDER_H
#define UI_SLIDER_H

#include "ui_single_value.h"

class UISlider : public UISingleValue {
    bool is_horizontal_;
    t_float knob_phase_prev_;
    t_float click_phase_;
    UIFont font_;
    UITextLayout txt_value_;

protected:
    t_rgba prop_knob_color;
    t_rgba prop_text_color;
    int prop_rel_mode;
    int prop_active_scale;
    t_symbol* prop_value_pos;
    int prop_value_precision;

public:
    UISlider();

    void init(t_symbol* name, const AtomList& args, bool usePresets);
    void paint();
    void okSize(t_rect* newrect);
    void onMouseDown(t_object*, const t_pt& pt, const t_pt& abs_pt, long modifiers);
    void onMouseDrag(t_object* view, const t_pt& pt, long modifiers);
    void onMouseUp(t_object* view, const t_pt& pt, long modifiers);
    void onDblClick(t_object* view, const t_pt& pt, long modifiers);

    void redrawKnob();

public:
    static void setup();

private:
    t_float calcValueAtMousePos(const t_pt& pt) const;
};

void setup_ui_slider();

#endif // UI_SLIDER_H

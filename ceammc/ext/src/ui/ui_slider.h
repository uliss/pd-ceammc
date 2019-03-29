#ifndef UI_SLIDER_H
#define UI_SLIDER_H

#include "ui_single_value.h"

class UISlider : public UISingleValue {
    bool is_horizontal_;
    float value_last_;
    float value_ref_;
    UIFont font_;
    UITextLayout txt_value_;

protected:
    t_rgba prop_knob_color;
    t_rgba prop_text_color;
    int prop_rel_mode;
    int mouse_up_output;
    int prop_active_scale;
    t_symbol* prop_value_pos;

public:
    UISlider();

    void init(t_symbol* name, const AtomList& args, bool usePresets);
    void paint(t_object*);
    void okSize(t_rect* newrect);
    void onMouseDown(t_object*, const t_pt& pt, long);
    void onMouseDrag(t_object* view, const t_pt& pt, long modifiers);
    void onMouseUp(t_object* view, const t_pt& pt, long modifiers);
    void onDblClick(t_object* view, const t_pt& pt, long modifiers);

public:
    static void setup();
};

void setup_ui_slider();

#endif // UI_SLIDER_H

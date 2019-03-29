#ifndef UI_KNOB_H
#define UI_KNOB_H

#include "ui_single_value.h"

using namespace ceammc;

class UIKnob : public UISingleValue {
    UIFont txt_font;
    UITextLayout txt_min;
    UITextLayout txt_max;

    int show_range_;
    int draw_active_scale_;
    t_pt click_pos_;
    UILayer knob_layer_;

public:
    t_rgba prop_knob_color;
    t_rgba prop_scale_color;

    static void setup();

public:
    UIKnob();

    void paint(t_object*);
    void okSize(t_rect* newrect);
    void onMouseDrag(t_object*, const t_pt& pt, long);
    void onMouseDown(t_object*, const t_pt& pt, long);
};

void setup_ui_knob();

#endif // UI_KNOB_H

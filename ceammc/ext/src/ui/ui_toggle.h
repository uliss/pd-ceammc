#ifndef UI_TOGGLE_H
#define UI_TOGGLE_H

#include "ceammc_ui_object.h"

using namespace ceammc;

class UIToggle : public UIObject {
    t_rgba prop_color_active;
    t_float prop_value_on_;
    t_float prop_value_off_;
    bool value_;

public:
    UIToggle();

    t_float value() const;
    void setValue(t_float v);
    void flip();
    void output();

    void okSize(t_rect* newrect);
    void paint();
    void onMouseDown(t_object*, const t_pt&, const t_pt& abs_pt, long);

    void onBang();
    void onFloat(t_float f);

    void m_set(t_float f);
    void loadPreset(size_t idx);
    void storePreset(size_t idx);

public:
    static void setup();

private:
    void redrawAll();
};

void setup_ui_toggle();

#endif // UI_TOGGLE_H

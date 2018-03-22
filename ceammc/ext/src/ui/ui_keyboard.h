#ifndef UI_KEYBOARD_H
#define UI_KEYBOARD_H

#include "ceammc_ui_object.h"

using namespace ceammc;

class UIKeyboard : public UIObject {
    int keys_;
    int shift_;
    int current_key_;
    int velocity_;
    bool mouse_pressed_;
    t_rgba prop_color_active_;

public:
    UIKeyboard();

    bool okSize(t_rect* newrect);
    void paint(t_object* view);

    void onMouseDown(t_object* view, const t_pt& pt, long modifiers);
    void onMouseUp(t_object* view, const t_pt& pt, long modifiers);
    void onMouseMove(t_object* view, const t_pt& pt, long modifiers);
    void onMouseLeave(t_object* view, const t_pt& pt, long modifiers);
    void onMouseDrag(t_object* view, const t_pt& pt, long modifiers);

    int findPressedKey(const t_pt& pt) const;

public:
    static void setup();

private:
    int realPitch() const;
    void output();
};

size_t keyboard_white_keys(size_t num_keys);

void setup_ui_keyboard();

#endif // UI_KEYBOARD_H

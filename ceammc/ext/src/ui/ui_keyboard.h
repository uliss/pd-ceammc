#ifndef UI_KEYBOARD_H
#define UI_KEYBOARD_H

#include "ceammc_ui_object.h"

#include <unordered_set>

using namespace ceammc;

class UIKeyboard : public UIObject {
    int keys_;
    int shift_;
    int current_key_;
    int velocity_;
    bool mouse_pressed_;
    t_rgba prop_color_active_;
    std::unordered_set<int> sustained_keys_;
    UILayer key_layer_;

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

    t_pd_err notify(t_symbol* attr_name, t_symbol* msg);

public:
    static void setup();

private:
    int realPitch() const;
    void output();
    void drawBackground();
    void drawActive();
};

/**
 * @brief keyboard_num_white_keys - number of white keys in keyboard.
 * @note assumed, that keyboard start from C
 * @param num_keys number of keys (starting from C-key)
 * @return number
 */
size_t keyboard_num_white_keys(size_t num_keys);

std::vector<t_pt> white_key_poly(int offset, float black_key_w, float key_h);

void setup_ui_keyboard();

#endif // UI_KEYBOARD_H

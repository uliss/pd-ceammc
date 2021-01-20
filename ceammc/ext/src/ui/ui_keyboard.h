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
    void paint();

    void onMouseDown(t_object* view, const t_pt& pt, const t_pt& abs_pt, long modifiers);
    void onMouseUp(t_object* view, const t_pt& pt, long modifiers);
    void onMouseMove(t_object* view, const t_pt& pt, long modifiers);
    void onMouseLeave(t_object* view, const t_pt& pt, long modifiers);
    void onMouseDrag(t_object* view, const t_pt& pt, long modifiers);
    void showPopup(const t_pt& pt, const t_pt& abs_pt);

    int findPressedKey(const t_pt& pt) const;
    void onList(const AtomListView& lv);

    const char* annotateInlet(int n) const;
    const char* annotateOutlet(int n) const;

public:
    static void setup();

private:
    void playChord(const std::unordered_set<int>& keys);
    int realPitch() const;
    void output();
    void drawBackground();
    void drawActive();
    void releaseAllNotes();
    void resetAllNotes();
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

#ifndef UI_KEYBOARD_H
#define UI_KEYBOARD_H

#include "ceammc_ui_object.h"

#include <bitset>
#include <cstdint>
#include <initializer_list>

using namespace ceammc;

class UIKeyboard : public UIObject {
    static const size_t MAX_KEYS = 88;

    int prop_keys;
    int prop_shift;
    int current_key_;
    int velocity_;
    int prop_vertical;
    bool mouse_pressed_;
    t_rgba prop_color_active_;
    std::bitset<MAX_KEYS> active_keys_;

public:
    UIKeyboard();

    bool okSize(::t_rect* newrect);
    void paint();
    void init(t_symbol* name, const AtomListView& args, bool usePresets);

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

    void playChord(const std::initializer_list<uint8_t>& keys);

public:
    static void setup();

private:
    int realPitch() const;
    void outputCurrentKey();
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

void setup_ui_keyboard();

#endif // UI_KEYBOARD_H

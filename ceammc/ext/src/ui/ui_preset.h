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
#ifndef UI_PRESET_H
#define UI_PRESET_H

#include "ceammc_ui_object.h"
#include <bitset>

using namespace ceammc;

class UIPreset : public UIObject {
    std::bitset<256> presets_;

private:
    t_rgba prop_color_text;
    t_rgba prop_color_empty;
    t_rgba prop_color_stored;
    t_rgba prop_color_active;

private:
    float button_size_;
    int selected_index_;
    int mouse_over_index_;

public:
    UIPreset();

    void init(t_symbol* name, const AtomListView& args, bool usePresets);
    void okSize(t_rect* newrect);
    void paint();

    void onMouseDown(t_object* view, const t_pt& pt, const t_pt& abs_pt, long modifiers);
    void onMouseMove(t_object* view, const t_pt& pt, long modifiers);
    void onMouseLeave(t_object* view, const t_pt& pt, long modifiers);

    int buttonIndexAt(float x, float y) const;

    void m_read(const AtomListView& lv);
    void m_write(const AtomListView& lv);
    void m_load(const AtomListView& lv);
    void m_store(const AtomListView& lv);
    void m_clear(const AtomListView& lv);
    void m_clearall(const AtomListView&);
    void m_duplicate(const AtomListView& lv);
    void m_interp(const AtomListView& lv);
    t_int propCurrent() const;

    bool hasPresetAt(size_t n) const { return presets_.test(n); }

public:
    static void setup();

private:
    void indexAdd(const AtomListView& lv);
    void indexRemove(const AtomListView& lv);
    void updateIndexes();
    void loadIndex(int idx);
    void storeIndex(int idx);
    void clearIndex(int idx);
    void interpIndex(t_float idx);
    bool checkIndex(float idx) const;
};

void setup_ui_preset();

#endif // UI_PRESET_H

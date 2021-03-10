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
#ifndef UI_TAB_H
#define UI_TAB_H

#include "ceammc_ui_object.h"

#include <bitset>
#include <memory>

using namespace ceammc;

typedef std::shared_ptr<UITextLayout> Layout;

class UITab : public UIObject {
    AtomList items_;
    int item_selected_;
    int item_hover_;
    std::vector<Layout> layouts_;
    std::vector<std::string> labels_;
    std::bitset<128> toggles_;

private:
    int prop_is_vertical;
    int prop_toggle_mode;
    t_rgba prop_color_text;
    t_rgba prop_color_hover;
    t_rgba prop_color_active;

public:
    UITab();

    void init(t_symbol* name, const AtomListView& args, bool usePresets);
    void okSize(t_rect* newrect);
    void paint();

    void onBang();
    void onFloat(t_float f);
    void onSymbol(t_symbol* s);
    void onAny(t_symbol* s, const AtomListView&);

    void onMouseDown(t_object* view, const t_pt& pt, const t_pt& abs_pt, long modifiers);
    void onMouseUp(t_object* view, const t_pt& pt, long modifiers);
    void onMouseMove(t_object* view, const t_pt& pt, long modifiers);
    void onMouseLeave(t_object* view, const t_pt& pt, long modifiers);

    void loadPreset(size_t idx);
    void storePreset(size_t idx);

    AtomList propItems() const;
    void propSetItems(const AtomListView& lv);
    AtomList propSelected() const;
    t_int propCount() const;
    t_int propCurrent() const;

    void m_append(const AtomListView& lv);
    void m_clear();
    void m_delete(t_float f);
    void m_insert(const AtomListView& lv);
    void m_select(const AtomListView& lv);
    void m_set_item(const AtomListView& lv);

private:
    bool isSelected(size_t idx) const;
    void output();
    bool selectSingle(int idx);
    int findIndex(const Atom& name);
    void syncLabels();

public:
    static void setup();
};

void setup_ui_tab();

#endif // UI_TAB_H

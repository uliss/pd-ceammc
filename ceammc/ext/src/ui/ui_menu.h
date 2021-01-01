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
#ifndef UI_MENU_H
#define UI_MENU_H

#include "ceammc_ui_object.h"

#include <memory>

using namespace ceammc;

typedef std::shared_ptr<UITextLayout> TextPtr;
typedef std::vector<std::string> LabelList;

class UIMenu : public UIObject {
    t_rgba prop_color_text;
    t_rgba prop_color_active;
    int prop_max_item_length;

private:
    AtomList items_;
    LabelList labels_;
    std::vector<TextPtr> layouts_;

private:
    bool is_open_;
    int item_height_;
    int current_idx_;
    int hover_idx_;
    int itemHeightZoomed() const { return item_height_ * zoom(); }

public:
    UIMenu();

    void init(t_symbol* name, const AtomList& args, bool usePresets);
    void okSize(t_rect* newrect);
    void paint();

    void onBang();
    void onFloat(t_float f);
    void onSymbol(t_symbol* s);
    void onAny(t_symbol* s, const AtomListView&);

    void onMouseDown(t_object* view, const t_pt& pt, const t_pt& abs_pt, long modifiers);
    void onMouseMove(t_object* view, const t_pt& pt, long modifiers);
    void onMouseLeave(t_object* view, const t_pt& pt, long modifiers);

    t_float propCount() const;
    AtomList propItems() const;
    void propSetItems(const AtomListView& lst);
    t_float propIndex() const;
    void propSetIndex(t_float f);
    AtomList propValue() const;
    void propSetValue(const AtomListView& lst);
    t_float propOpen() const;
    void propSetOpen(t_float v);
    AtomList propTitle() const;

    void loadPreset(size_t idx);
    void storePreset(size_t idx);

    void m_append(const AtomListView& lst);
    void m_clear();
    void m_delete(t_float idx);
    void m_set(const AtomListView& lst);
    void m_insert(const AtomListView& lst);
    void m_set_item(const AtomListView& lst);
    void m_file_glob(const AtomListView& lst);

private:
    void syncLabels();
    int findIndex(int y);
    bool selectByValue(const AtomListView& lst);
    void output();
    void setOpen(bool v = true);
    void adjustSize();

public:
    static void setup();
};

void setup_ui_menu();

#endif // UI_MENU_H

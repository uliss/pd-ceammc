/*****************************************************************************
 * Copyright 2023 Serge Poltavski. All rights reserved.
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
#ifndef UI_2D_MAP_H
#define UI_2D_MAP_H

#include "ceammc_ui_object.h"

#include "Hoa_Source.hpp"

#include <array>
#include <map>

namespace ceammc {

struct t_linkmap;

class Hoa2dMapUI : public UIObject {
    hoa::Source::Manager* f_manager;
    hoa::Source::Manager* f_self_manager;

    hoa::Source* f_selected_source;
    hoa::Source::Group* f_selected_group;

    t_pt f_cursor_position;

    float f_zoom_factor;

    t_rect f_rect_selection;
    int f_rect_selection_exist;

    t_symbol* f_output_mode;
    uint32_t f_read;
    uint32_t f_write;

    int f_mouse_was_dragging;

    t_symbol* f_coord_view;

    t_symbol* f_binding_name;
    t_linkmap* f_listmap;
    int f_output_enabled;
    friend struct t_linkmap;

    UILayer sources_, groups_, selection_;

public:
    Hoa2dMapUI();
    ~Hoa2dMapUI();

    void okSize(t_rect* newrect);
    void paint();

    void onList(const AtomListView& lv);

    void onMouseDown(t_object* view, const t_pt& pt, const t_pt& abs_pt, long modifiers);
    void onMouseUp(t_object* view, const t_pt& pt, long modifiers);
    void onMouseDrag(t_object* view, const t_pt& pt, long modifiers);
    void onMouseMove(t_object* view, const t_pt& pt, long modifiers);

    void showPopup(const t_pt& pt, const t_pt& abs_pt);

    // presets
    void loadPreset(size_t idx);
    void storePreset(size_t idx);
    void interpPreset(t_float idx);
    bool hasPresetInterp() const { return true; }

    void m_source(const AtomListView& lv);
    void m_group(const AtomListView& lv);
    void m_clear_all(const AtomListView& lv);
    void m_set(const AtomListView& lv);

    AtomList m_get_zoom() const { return Atom(f_zoom_factor); }
    void m_set_zoom(const AtomListView& lv);

    void m_set_bind(const AtomListView& lv);

public:
    static void setup();

private:
    void drawBackground();
    void drawSelection();
    void drawSources();
    void drawGroups();
    void output();

    void linkmapAddWithBindingName(t_symbol* name);
    void linkmapRemoveWithBindingName(t_symbol* binding_name);
    void sendBindedMapUpdate(long flags);
    void selectElement(const t_pt& pt);
    void ctl_source(const AtomListView& lv);
    void ctl_group(const AtomListView& lv);
};

}

void setup_spat_hoa_2d_map_ui();

#endif // UI_2D_MAP_H

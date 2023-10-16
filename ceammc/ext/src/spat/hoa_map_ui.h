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
#ifndef HOA_MAP_UI_H
#define HOA_MAP_UI_H

#include "ceammc_ui_object.h"

#include "Hoa_Source.hpp"

#include <array>
#include <forward_list>
#include <map>
#include <memory>

namespace ceammc {

struct HoaManagerData;
using SharedManager = std::shared_ptr<HoaManagerData>;

class HoaMapUI : public UIObject {
    SharedManager f_manager;

    hoa::Source* f_selected_source;
    hoa::Source::Group* f_selected_group;

    t_symbol* f_output_mode;
    t_symbol* f_coord_view;
    t_symbol* f_binding_name;

    t_pt f_cursor_position;
    t_rect f_rect_selection;
    t_rgba prop_selection_color;

    UILayer sources_, groups_, selection_;
    float prop_zoom;

    bool output_enabled_ { true };
    bool mouse_was_dragging_ { false };
    bool f_rect_selection_exist { false };

public:
    HoaMapUI();
    ~HoaMapUI();

    void okSize(t_rect* newrect);
    void paint();

    void onBang();

    void onMouseDown(t_object* view, const t_pt& pt, const t_pt& abs_pt, long modifiers);
    void onMouseUp(t_object* view, const t_pt& pt, long modifiers);
    void onMouseDrag(t_object* view, const t_pt& pt, long modifiers);
    void onMouseMove(t_object* view, const t_pt& pt, long modifiers);
    void onMouseWheel(const t_pt& pt, long modifiers, float delta);

    void showPopup(const t_pt& pt, const t_pt& abs_pt);

    // presets
    void loadPreset(size_t idx);
    void storePreset(size_t idx);
    void interpPreset(t_float idx);
    bool hasPresetInterp() const { return true; }

    void m_clear_all(const AtomListView& lv);
    void m_set(const AtomListView& lv);
    void m_source(const AtomListView& lv);
    void m_group(const AtomListView& lv);
    void m_info();

    AtomList m_get_zoom() const { return Atom(prop_zoom); }
    void m_set_zoom(const AtomListView& lv);

    void m_set_bind(const AtomListView& lv);
    AtomList m_get_bind() const { return Atom(f_binding_name); }

public:
    static void setup();

private:
    void drawBackground();
    void drawSelection();
    void drawSources();
    void drawGroups();
    void output();

    void sendBindedMapUpdate(long flags);
    void selectElement(const t_pt& pt);

    void updateAllAndOutput();
    t_symbol* makeBindSymbol(t_symbol* sym) const;
    AtomList serializeSource(const hoa::Source& src) const;
};

}

void setup_spat_hoa_map_ui();

#endif // HOA_MAP_UI_H

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
#ifndef HOA_2D_SPACE_H
#define HOA_2D_SPACE_H

#include "ceammc_ui_object.h"
#include "hoa_common.h"

#include <array>

namespace ceammc {

class Hoa2dSpace : public UIObject {
    t_rgba prop_space_color, prop_pt_color;
    t_float prop_min { 0 }, prop_max { 1 };
    int prop_nchan { 0 };
    std::array<float, HOA_MAX_PLANEWAVES> chan_values_, chan_refs_, chan_radius_;

    UILayer space_layer_, points_layer_;
    float angle_ref_, value_ref_;

    enum EditMode {
        EDIT_DEFAULT,
        EDIT_ROTATE,
        EDIT_ZOOM
    };

    EditMode mode_ { EDIT_DEFAULT };

public:
    Hoa2dSpace();

    void init(t_symbol* s, const AtomListView& args, bool usePresets);

    void okSize(t_rect* newrect);
    void paint();

    t_float propNumChan() const;
    void propSetNumChan(t_float v);

    void onList(const AtomListView& lv);

    void onMouseDown(t_object* view, const t_pt& pt, const t_pt& abs_pt, long modifiers);
    void onMouseDrag(t_object* view, const t_pt& pt, long modifiers);

    // presets
    void loadPreset(size_t idx);
    void storePreset(size_t idx);
    void interpPreset(t_float idx);
    bool hasPresetInterp() const { return true; }

public:
    static void setup();

private:
    void drawBackground();
    void drawSpace();
    void drawPoints();
    void output();
};

}

void setup_spat_hoa_2d_space();

#endif // HOA_2D_SPACE_H

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
#ifndef UI_ICON_H
#define UI_ICON_H

#include "ceammc_clock.h"
#include "ceammc_ui_object.h"

using namespace ceammc;

struct icon_entry;

class UIIcon : public UIObject {
    ClockMemberFunction<UIIcon> clock_;
    UILayer image_layer_;
    t_eimage* image_;
    icon_entry* current_;
    t_symbol* prop_icon;
    t_symbol* prop_mode;
    t_rgba prop_color_active;
    int prop_size;
    bool is_active_;
    bool is_enabled_;

public:
    UIIcon();
    ~UIIcon();

    void init(t_symbol* name, const AtomList& args, bool usePresets);
    void onPropChange(t_symbol* prop_name);
    void okSize(t_rect* newrect);
    void paint();
    void drawBackground();
    void drawImage();

    void onBang();
    void onFloat(t_float f);

    void onMouseDown(t_object* view, const t_pt& pt, const t_pt& abs_pt, long modifiers);
    void onMouseUp(t_object* view, const t_pt& pt, long modifiers);

    void m_set(const AtomListView& lst);

    void loadPreset(size_t idx);
    void storePreset(size_t idx);

public:
    static void setup();
    void updateIconProp();
    void outputBang();
    void outputState(bool state);

private:
    bool isSpecialIcon() const;
    void clockTick();
    void redrawAll();

    AtomList propEnabled() const;
    void propSetEnabled(const AtomListView& lst);
};

void setup_ui_icon();

#endif // UI_ICON_H

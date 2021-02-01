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
#ifndef UI_COLORPANEL_H
#define UI_COLORPANEL_H

#include "ceammc_ui_object.h"

using namespace ceammc;

typedef std::vector<t_rgba> RgbCol;
typedef std::vector<RgbCol> RgbMatrix;

class UIColorPanel : public UIObject {
    int matrix_x_;
    int matrix_y_;
    RgbMatrix colors_;

    UILayer hover_layer_;
    UILayer picked_layer_;

    t_float saturation_;
    t_float hue_;
    t_float lightness_;

    int prop_reverse_;

    int picked_x_;
    int picked_y_;
    int hover_x_;
    int hover_y_;

public:
    UIColorPanel();

    void okSize(t_rect* newrect);
    void onPropChange(t_symbol* prop_name);
    void paint();
    void drawBackground();
    void drawHover();
    void drawPicked();

    void onMouseDown(t_object* view, const t_pt& pt, const t_pt& abs_pt, long modifiers);
    void onMouseLeave(t_object* view, const t_pt& pt, long modifiers);
    void onMouseMove(t_object* view, const t_pt& pt, long modifiers);
    void onBang();

    AtomList propMatrixSize() const;
    void propSetMatrixSize(const AtomListView& lst);
    AtomList propColorIndex() const;

    t_float propPdColor() const;
    AtomList propRgbColor() const;
    AtomList propHslColor() const;
    AtomList propHexColor() const;

    void m_set(const AtomListView& lv);

    void loadPreset(size_t idx);
    void storePreset(size_t idx);

private:
    void computeColors();
    void redrawAll();
    void output();

public:
    static void setup();
};

void setup_ui_colorpanel();

#endif // UI_COLORPANEL_H

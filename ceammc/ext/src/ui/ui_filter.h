/*****************************************************************************
 * Copyright 2021 Serge Poltavsky. All rights reserved.
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
#ifndef UI_FILTER_H
#define UI_FILTER_H

#include "ceammc_ui_object.h"

#include <array>

using namespace ceammc;

class UIFilter : public UIObject {
    t_float b0_, b1_, b2_, a1_, a2_;
    t_pt freq_pt_;
    t_symbol* prop_type;
    t_float peak_q_;

public:
    UIFilter();

    bool okSize(t_rect* newrect);
    void paint();

    void onList(const AtomListView& lv);

    void onMouseDown(t_object* view, const t_pt& pt, const t_pt& abs_pt, long modifiers);
    void onMouseUp(t_object* view, const t_pt& pt, long modifiers);
    void onMouseDrag(t_object* view, const t_pt& pt, long modifiers);
    void onMouseWheel(const t_pt& pt, long modifiers, float delta);

public:
    using Array = std::array<double, 6>;

private:
    void calc();
    float calcFrequency() const;
    float calcQ() const;
    float calcDb() const;
    void saveMousePoint(const t_pt& pt);
    void output();
    void setBA(const Array& ba);

public:
    static void setup();
};

void setup_ui_filter();

#endif // UI_FILTER_H

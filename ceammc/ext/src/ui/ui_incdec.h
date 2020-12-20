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
#ifndef UI_INCDEC_H
#define UI_INCDEC_H

#include "ceammc_ui_object.h"

using namespace ceammc;

class UIIncDec : public UIObject {
    t_rgba prop_color_arrow;
    t_float prop_step;

private:
    t_float value_;
    int mouse_down_;

public:
    UIIncDec();

    void okSize(t_rect* newrect);
    void paint();

    void onBang();
    void onFloat(t_float f);

    void onMouseDown(t_object* view, const t_pt& pt, const t_pt& abs_pt, long modifiers);
    void onMouseUp(t_object* view, const t_pt& pt, long modifiers);

    void m_inc();
    void m_dec();

    void loadPreset(size_t idx);
    void storePreset(size_t idx);

public:
    static void setup();

private:
    void output();
    t_float propValue() const;
    void propSetValue(t_float f);
};

void setup_ui_incdec();

#endif // UI_INCDEC_H

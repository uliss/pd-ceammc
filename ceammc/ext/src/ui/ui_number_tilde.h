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
#ifndef UI_NUMBER_TILDE_H
#define UI_NUMBER_TILDE_H

#include "ceammc_clock.h"
#include "ceammc_ui_object.h"

using namespace ceammc;

class UINumberTilde : public UIDspObject {
    static const size_t BUFSIZE = 32;

    ClockLambdaFunction clock_;
    t_float value_;
    double last_redraw_time_;
    char text_value_[BUFSIZE];

private:
    t_rgba prop_color_text;
    t_rgba prop_color_active;
    int prop_max_decimal;
    int prop_interval;

public:
    UINumberTilde();

    void init(t_symbol* name, const AtomListView& args, bool usePresets);
    void okSize(t_rect* newrect);
    void paint();

    void dspProcess(t_sample** ins, long n_ins, t_sample** outs, long n_outs, long sampleframes);

public:
    static void setup();

private:
    void updateTextValue();
};

void setup_ui_number_tilde();

#endif // UI_NUMBER_TILDE_H

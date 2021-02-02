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
#ifndef UI_METER_H
#define UI_METER_H

#include "ceammc_clock.h"
#include "ceammc_ui_object.h"

using namespace ceammc;

class UIMeter : public UIDspObject {
    ClockMemberFunction<UIMeter> clock_;
    t_rgba prop_color_cold;
    t_rgba prop_color_tepid;
    t_rgba prop_color_warm;
    t_rgba prop_color_hot;
    t_rgba prop_color_over;
    int prop_interval_ms;

private:
    t_float raw_peak_;
    double raw_square_sum_;
    size_t num_samples_;
    int overload_;
    t_float rms_dbfs_;
    t_float peak_dbfs_;
    bool start_clock_;
    bool is_horizontal_;

public:
    UIMeter();

    void init(t_symbol* name, const AtomListView& args, bool usePresets);
    void okSize(t_rect* newrect);
    void paint();

    void dspInit();
    void dspOn(double samplerate, long blocksize);
    void dspProcess(t_sample** ins, long n_ins, t_sample** outs, long n_outs, long sampleframes);

    const char* annotateInlet(int n) const;
    const char* annotateOutlet(int n) const;

    void onDblClick(t_object* view, const t_pt& pt, long modifiers);

public:
    static void setup();

private:
    void calc();
    void reset();
    void clockTick();
    void output();
};

void setup_ui_meter();

#endif // UI_METER_H

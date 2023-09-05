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
#ifndef UI_CIRCULAR_METER_H
#define UI_CIRCULAR_METER_H

#include "ceammc_clock.h"
#include "ceammc_ui_object.h"

#include <array>

using namespace ceammc;

class UICircularMeter : public UIDspObject {
public:
    constexpr static int MIN_NCHAN = 1;
    constexpr static int MAX_NCHAN = 64;
    constexpr static int DEF_NCHAN = 2;

    constexpr static int MIN_OFFSET = 1 - MAX_NCHAN;
    constexpr static int MAX_OFFSET = MAX_NCHAN - 1;
    constexpr static int DEF_OFFSET = 0;

private:
    ClockLambdaFunction clock_;
    t_rgba prop_color_cold;
    t_rgba prop_color_tepid;
    t_rgba prop_color_warm;
    t_rgba prop_color_hot;
    t_rgba prop_color_over;
    int prop_interval_ms;
    int prop_nchan;
    int prop_offset;
    int prop_clockwise;
    AtomList prop_angles;

private:
    std::array<float, MAX_NCHAN> raw_peak_;
    std::array<float, MAX_NCHAN> raw_sum2_;
    std::array<std::uint16_t, MAX_NCHAN> out_over_;
    std::array<std::int8_t, MAX_NCHAN> out_peak_;
    std::array<std::int8_t, MAX_NCHAN> out_rms_;
    std::array<float, MAX_NCHAN> out_angles_;

    size_t num_samples_;
    bool start_clock_;

public:
    UICircularMeter();

    void init(t_symbol* name, const AtomListView& args, bool usePresets);
    void okSize(t_rect* newrect);
    void paint();

    void dspInit();
    void dspOn(double samplerate, long blocksize);
    void dspProcess(t_sample** ins, long n_ins, t_sample** outs, long n_outs, long sampleframes);

    t_int propNumChan() const { return prop_nchan; }
    t_int propOffset() const { return prop_offset; }
    AtomList propAngles() const { return prop_angles; }

    void propSetNumChan(t_int n);
    void propSetOffset(t_int off);
    void propSetAngles(const AtomListView& lv);

public:
    static void setup();

private:
    void calc();
    void reset();
    void setInputs(int n);
};

void setup_ui_circular_meter();

#endif // UI_CIRCULAR_METER_H

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
#ifndef HOA_3D_METER_H
#define HOA_3D_METER_H

#include <memory>

#include "ceammc_clock.h"
#include "ceammc_ui_object.h"
#include "hoa_common.h"
using namespace ceammc;

class Hoa3dMeter : public UIDspObject {
    std::unique_ptr<Meter3d> f_meter;
    std::unique_ptr<Vector3d> f_vector;

    std::unique_ptr<t_sample, t_sample* (*)(t_sample*)> f_signals;
    t_sample f_vector_coords[6];
    long f_ramp;
    int f_startclock;
    int f_interval;

    t_symbol* f_vector_type;
    t_symbol* f_view;
    int f_clockwise;

    t_rgba f_color_cold_signal;
    t_rgba f_color_tepid_signal;
    t_rgba f_color_warm_signal;
    t_rgba f_color_hot_signal;
    t_rgba f_color_over_signal;
    t_rgba f_color_energy_vector;
    t_rgba f_color_velocity_vector;

    double f_radius;
    double f_center;
    double f_radius_center;

    ClockLambdaFunction f_clock;

    // my
    int prop_channels;
    UILayer led_layer, vec_layer;

public:
    Hoa3dMeter();

    void init(t_symbol* s, const AtomListView& args, bool usePresets);
    void okSize(t_rect* newrect);
    void paint();
    void dspProcess(t_sample** ins, long n_ins, t_sample** outs, long n_outs, long sampleframes);
    void dspOn(double samplerate, long blocksize);

    t_int propChannels() const;
    void propSetChannels(t_int n);

    AtomList propAngles() const;
    void propSetAngles(const AtomListView& lv);

    AtomList propOffset() const;
    void propSetOffset(const AtomListView& lv);

    t_symbol* propVectors() const { return f_vector_type; }
    void propSetVectors(t_symbol* type);

    t_int propRotation() const { return f_clockwise; }
    void propSetRotation(t_int s);

    t_symbol* propView() const { return f_view; }
    void propSetView(t_symbol* view);

    void clockTick();

public:
    void m_beatles();

public:
    static void setup();

private:
    const t_rgba& segmentColor(bool peak, float db) const;
    void drawLeds();
    void drawVectors();
    void drawBackground();

    t_rect& rawRect() { return asEBox()->b_rect; }
    t_rect& rawLastRect() { return asEBox()->b_rect_last; }
};

void setup_spat_hoa_3d_meter();

#endif // HOA_3D_METER_H

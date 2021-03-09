/*****************************************************************************
 * Copyright 2019 Serge Poltavsky. All rights reserved.
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
#ifndef HOA_2D_SCOPE_H
#define HOA_2D_SCOPE_H

#include <memory>

#include "ceammc_clock.h"
#include "ceammc_ui_object.h"
#include "hoa_common.h"
using namespace ceammc;

class Hoa2dScope : public UIDspObject {
    size_t nharm_;
    std::unique_ptr<Scope2d> scope_;
    Buffer in_buf_;
    int prop_order_;
    t_float prop_gain_;
    t_float prop_view_;
    int prop_refresh_;
    t_rgba prop_ph_color_;
    t_rgba prop_nh_color_;

    ClockMemberFunction<Hoa2dScope> clock_;
    bool start_clock_;

    UILayer harm_layer_;

public:
    Hoa2dScope();

    void init(t_symbol* s, const AtomListView& args, bool usePresets);

    void okSize(t_rect* newrect);
    void paint();

    t_float propOrder() const;
    void propSetOrder(t_float v);
    t_float propView() const;
    void propSetView(t_float angle);

    void tick();

    // dsp
    void dspOn(double samplerate, long blocksize);
    void dspProcess(t_sample** ins, long n_ins, t_sample** outs, long n_outs, long sampleframes);

public:
    static void setup();

private:
    void drawBackground();
    void drawHarmonics();
};

void setup_spat_hoa_scope2d();

#endif // HOA_2D_SCOPE_H

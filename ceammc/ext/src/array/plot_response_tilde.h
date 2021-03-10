/*****************************************************************************
 * Copyright 2020 Serge Poltavsky. All rights reserved.
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
#ifndef PLOT_RESPONSE_TILDE_H
#define PLOT_RESPONSE_TILDE_H

#include "ceammc_clock.h"
#include "ceammc_sound_external.h"

using namespace ceammc;

class PlotResponseTilde : public SoundExternal {
    IntProperty* n_;
    BoolProperty* dbfs_;
    BoolProperty* use_sr_;
    std::vector<t_sample> real_, imag_;
    size_t phase_;
    ClockLambdaFunction clock_;

    enum State {
        IDLE,
        CALC,
        OUTPUT
    };

    State state_ = IDLE;

public:
    PlotResponseTilde(const PdArgs& a);

    void onInlet(size_t, const AtomListView&) override;
    void processBlock(const t_sample** in, t_sample** out) final;
};

void setup_plot_response_tilde();

#endif // PLOT_RESPONSE_TILDE_H

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
#ifndef PLOT_HIST_TILDE_H
#define PLOT_HIST_TILDE_H

#include "ceammc_clock.h"
#include "ceammc_sound_external.h"
#include <cstdint>

using namespace ceammc;

class PlotHistTilde : public SoundExternal {
    IntProperty* nsamp_;
    IntProperty* nbins_;
    FloatProperty* min_;
    FloatProperty* max_;
    BoolProperty* clip_;
    std::vector<uint32_t> buf_;
    ClockLambdaFunction clock_;
    int phase_;

    enum State {
        IDLE,
        CALC,
        OUTPUT
    };

    State state_ = IDLE;

public:
    PlotHistTilde(const PdArgs& args);

    void onBang() override;
    void processBlock(const t_sample** in, t_sample** out) override;
};

void setup_plot_hist_tilde();

#endif // PLOT_HIST_TILDE_H

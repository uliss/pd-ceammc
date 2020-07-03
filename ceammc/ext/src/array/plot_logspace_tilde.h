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
#ifndef PLOT_LOG_TILDE_H
#define PLOT_LOG_TILDE_H

#include "ceammc_clock.h"
#include "ceammc_property_enum.h"
#include "ceammc_sound_external.h"

#include <cmath>

using namespace ceammc;

class PlotLogTilde : public SoundExternal {
    FloatProperty* start_;
    FloatProperty* stop_;
    IntProperty* num_;
    BoolProperty* endpoint_;
    t_sample value_;
    t_sample fbase_;
    t_sample incr_;
    int phase_;
    bool running_;
    EnumProperty<Atom>* base_;

public:
    PlotLogTilde(const PdArgs& a);

    void onBang() override;
    void onFloat(t_float f) override;
    void processBlock(const t_sample** in, t_sample** out) override;
};

void setup_plot_logspace_tilde();

#endif // PLOT_LOG_TILDE_H

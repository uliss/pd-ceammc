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
#ifndef ARRAY_PLOT_TILDE_H
#define ARRAY_PLOT_TILDE_H

#include "ceammc_array.h"
#include "ceammc_clock.h"
#include "ceammc_sound_external.h"

using namespace ceammc;

class ArrayPlotTilde : public SoundExternal {
    t_symbol* array_name_;
    Array array_;
    ClockLambdaFunction clock_;
    FloatProperty *ymin_, *ymax_;
    BoolProperty* yauto_;
    size_t phase_;
    bool running_;
    long total_;
    t_sample min_, max_;

public:
    ArrayPlotTilde(const PdArgs& a);

    void onInlet(size_t n, const AtomList& l) override;

    void setupDSP(t_signal** sp) override;
    void processBlock(const t_sample** in, t_sample** out) override;

    bool setArray(t_symbol* s);
    bool checkArray(bool log);

private:
    void updateScale();
};

void setup_array_plot_tilde();

#endif // ARRAY_PLOT_TILDE_H

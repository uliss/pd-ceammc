/*****************************************************************************
 * Copyright 2021 Serge Poltavsky. All rights reserved.
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
#ifndef CONV_SIG2FLOAT_H
#define CONV_SIG2FLOAT_H

#include "ceammc_clock.h"
#include "ceammc_sound_external.h"
using namespace ceammc;

class ConvSig2Float : public SoundExternal {
    ClockLambdaFunction clock_;
    FloatProperty* interval_ms_;
    IntProperty* block_offset_;
    size_t sample_counter_, period_;
    t_float value_;

public:
    ConvSig2Float(const PdArgs& args);

    void onBang() override;
    void onClick(t_floatarg xpos, t_floatarg ypos, t_floatarg shift, t_floatarg ctrl, t_floatarg alt) override;
    void onInlet(size_t n, const AtomListView& lv) override;
    void setupDSP(t_signal** sp) final;
    void processBlock(const t_sample** in, t_sample**) final;
};

void setup_conv_sig2float();

#endif // CONV_SIG2FLOAT_H

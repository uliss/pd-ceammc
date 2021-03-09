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
#include "ui_filter_tilde.h"
#include "ceammc_ui.h"

constexpr double SMOOTH_TIME_MS = 20;

UIFilterTilde::UIFilterTilde()
    : FilterBase(1)
{
}

void UIFilterTilde::init(t_symbol* name, const AtomListView& args, bool usePresets)
{
    FilterBase::init(name, args, usePresets);
    dspSetup(1, 1);
    createOutlet();
}

void UIFilterTilde::dspOn(double samplerate, long blocksize)
{
    sm0_.setSmoothTime(SMOOTH_TIME_MS, samplerate);
    sm1_.setSmoothTime(SMOOTH_TIME_MS, samplerate);
    sm2_.setSmoothTime(SMOOTH_TIME_MS, samplerate);
    sm3_.setSmoothTime(SMOOTH_TIME_MS, samplerate);
    sm4_.setSmoothTime(SMOOTH_TIME_MS, samplerate);

    UIDspObject::dspOn(samplerate, blocksize);
}

void UIFilterTilde::dspProcess(t_sample** ins, long n_ins, t_sample** outs, long n_outs, long bs)
{
    t_sample* in = ins[0];
    t_sample* out = outs[0];

#define PROCESS(offset)                                             \
    {                                                               \
        out[i + offset] = process(in[i + offset],                   \
            sm0_(b0_), sm1_(b1_), sm2_(b2_), sm3_(a1_), sm4_(a2_)); \
    }

    for (long i = 0; i < bs; i += 8) {
        PROCESS(0);
        PROCESS(1);
        PROCESS(2);
        PROCESS(3);
        PROCESS(4);
        PROCESS(5);
        PROCESS(6);
        PROCESS(7);
    }

#undef PROCESS
}

const char* UIFilterTilde::annotateOutlet(int n) const
{
    if (n == 0)
        return "signal: output";
    else
        return "list: biquad coefficients - b0 b1 b2 a1 a2";
}

void setup_ui_filter_tilde()
{
    UIObjectFactory<UIFilterTilde> obj("ui.filter~", EBOX_GROWINDI);

    UIFilterTilde::setup(obj);
}

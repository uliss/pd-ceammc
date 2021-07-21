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
#include "samp_time_tilde.h"
#include "ceammc_factory.h"

SampTimeTilde::SampTimeTilde(const PdArgs& args)
    : SoundExternal(args)
    , t_(0)
    , in_prev_(0)
    , on_(true)
{
    createInlet();
    createSignalOutlet();
}

void SampTimeTilde::processBlock(const t_sample** in, t_sample** out)
{
    const size_t BS = blockSize();

    for (size_t i = 0; i < BS; i++) {
        auto trig = static_cast<int>(in[0][i]);

        const auto trig_up = (in_prev_ == 0 && trig > 0);
        const auto trig_down = (in_prev_ > 0 && trig == 0);

        if (trig_up) {
            on_ = true;
            t_ = 0;
        } else if (trig_down)
            on_ = false;

        if (on_)
            out[0][i] = t_++;
        else
            out[0][i] = t_;

        in_prev_ = trig;
    }
}

void SampTimeTilde::onInlet(size_t n, const AtomListView& lv)
{
    if (lv.empty())
        t_ = 0;
    else if (lv.isFloat())
        t_ = lv[0].asT<t_float>();
}

void setup_samp_time_tilde()
{
    SoundExternalFactory<SampTimeTilde> obj("samp.time~");
}

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
#include "osc_blit.h"
#include "ceammc_convert.h"
#include "ceammc_factory.h"

#include "Blit.h"

constexpr t_float DEFAULT_FREQ = 440;
constexpr size_t DEFAULT_NHARM = 0;

OscBlit::OscBlit(const PdArgs& args)
    : SoundExternal(args)
    , osc_(new stk::Blit(parsedPosArgs().floatAt(0, DEFAULT_FREQ)))
    , nharm_(nullptr)
{
    nharm_ = new SizeTProperty("@harmonics", DEFAULT_NHARM);
    nharm_->setArgIndex(1);
    addProperty(nharm_);

    createInlet();
    createSignalOutlet();
}

void OscBlit::processBlock(const t_sample** in, t_sample** out)
{
    const size_t BS = blockSize();

    const t_float block_freq = std::fabs(in[0][0]);

    if (block_freq > 0) {
        constexpr int MAX_NHARM = 1000;
        const size_t MAX_FREQ = samplerate() / 2;
        auto calc_n = clip<int, 0, MAX_NHARM>(MAX_FREQ / int(block_freq));
        auto n = std::min<int>(nharm_->value(), calc_n);
        osc_->setHarmonics(n);
    }

    for (size_t i = 0; i < BS; i++) {
        osc_->setFrequency(std::fabs(in[0][i]));
        out[0][i] = osc_->tick();
    }
}

void OscBlit::setupDSP(t_signal** sp)
{
    SoundExternal::setupDSP(sp);
    stk::Stk::setSampleRate(samplerate());
}

void OscBlit::onInlet(size_t n, const AtomListView& l)
{
    osc_->reset();
}

void setup_osc_blit_tilde()
{
    SoundExternalFactory<OscBlit> obj("osc.blit~");
}

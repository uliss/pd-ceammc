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

static const t_float DEFAULT_FREQ = 440;

OscBlit::OscBlit(const PdArgs& args)
    : SoundExternal(args)
    , osc_(nullptr)
    , freq_(positionalFloatArgument(0, DEFAULT_FREQ))
    , num_harmonics_(0)
{
    auto pfreq = createCbProperty("@freq", &OscBlit::propFreq, &OscBlit::propSetFreq);
    pfreq->info().setType(PropertyInfoType::FLOAT);
    pfreq->info().setMin(0);
    pfreq->info().setDefault(DEFAULT_FREQ);
    pfreq->info().setUnits(PropertyInfoUnits::HZ);

    auto pharm = createCbProperty("@harmonics", &OscBlit::propHarm, &OscBlit::propSetHarm);
    pharm->info().setType(PropertyInfoType::INTEGER);

    osc_ = new stk::Blit(freq_);

    createInlet();
    createSignalOutlet();

    if (positionalFloatArgument(1) > 0)
        num_harmonics_ = positionalFloatArgument(1);
}

OscBlit::~OscBlit()
{
    delete osc_;
}

void OscBlit::processBlock(const t_sample** in, t_sample** out)
{
    const size_t BS = blockSize();
    const size_t SR = samplerate();

    t_float block_freq = std::abs(in[0][0]);
    // if signal
    if (block_freq > 0)
        freq_ = block_freq;

    if (freq_ > 0) {
        auto calc_n = clip<int>((SR / 2) / int(freq_), 0, 1000);
        auto n = std::min<int>(num_harmonics_, calc_n);
        osc_->setHarmonics(n);
    }

    for (size_t i = 0; i < BS; i++) {
        auto fs = std::abs(in[0][i]);
        // no signal
        if (fs == 0)
            fs = freq_;
        else
            freq_ = fs;

        osc_->setFrequency(freq_);
        out[0][i] = osc_->tick();
    }
}

void OscBlit::setupDSP(t_signal** sp)
{
    SoundExternal::setupDSP(sp);
    stk::Stk::setSampleRate(samplerate());
}

void OscBlit::onInlet(size_t n, const AtomList& l)
{
    osc_->reset();
}

AtomList OscBlit::propFreq() const
{
    return Atom(freq_);
}

void OscBlit::propSetFreq(const AtomList& lst)
{
    freq_ = lst.floatAt(0, 0);
    osc_->setFrequency(std::fabs(freq_));
}

AtomList OscBlit::propHarm() const
{
    return Atom(num_harmonics_);
}

void OscBlit::propSetHarm(const AtomList& lst)
{
    if (!checkArgs(lst, ARG_NATURAL))
        return;

    num_harmonics_ = lst[0].asInt();
}

void setup_osc_blit_tilde()
{
    SoundExternalFactory<OscBlit> obj("osc.blit~");
}

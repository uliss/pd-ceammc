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
#include "ceammc_factory.h"

#include "Blit.h"

OscBlit::OscBlit(const PdArgs& args)
    : SoundExternal(args)
    , osc_(nullptr)
    , freq_(positionalFloatArgument(0, 440))
{
    createCbProperty("@freq", &OscBlit::propFreq, &OscBlit::propSetFreq);

    osc_ = new stk::Blit(freq_);

    createInlet();
    createSignalOutlet();
}

OscBlit::~OscBlit()
{
    delete osc_;
}

void OscBlit::processBlock(const t_sample** in, t_sample** out)
{
    const size_t BS = blockSize();

    for (size_t i = 0; i < BS; i++) {
        freq_ = in[0][i];
        osc_->setFrequency(freq_);
        out[0][i] = osc_->tick();
    }
}

void OscBlit::setupDSP(t_signal** sp)
{
    SoundExternal::setupDSP(sp);
    stk::Stk::setSampleRate(samplerate());
}

AtomList OscBlit::propFreq() const
{
    return Atom(freq_);
}

void OscBlit::propSetFreq(const AtomList& lst)
{
    freq_ = lst.floatAt(0, 0);
    osc_->setFrequency(freq_);
}

void setup_osc_blit()
{
    SoundExternalFactory<OscBlit> obj("osc.blit~");
}

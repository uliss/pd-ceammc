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
#include "flow_demultiplex_tilde.h"
#include "ceammc_convert.h"
#include "ceammc_factory.h"

constexpr size_t DEFAULT_OUTLETS = 2;
constexpr size_t MIN_OUTLETS = 2;
constexpr size_t MAX_OUTLETS = 16;

static size_t chMultiplier(const PdArgs& args)
{
    return (args.flags & DEMULTIPLEX_STEREO) ? 2 : 1;
}

DemultiplexTilde::DemultiplexTilde(const PdArgs& args)
    : SoundExternal(args)
{
    const size_t NCHAN = positionalConstant<DEFAULT_OUTLETS, MIN_OUTLETS, MAX_OUTLETS>(0);
    for (size_t i = 0; i < NCHAN * chMultiplier(args); i++)
        createSignalOutlet();

    if (args.flags == DEMULTIPLEX_STEREO)
        createSignalInlet();

    createInlet();

    gain_.assign(NCHAN, t_smooth(0));
    gain_[0].setTargetValue(1);

    createCbProperty("@value", &DemultiplexTilde::propValue, &DemultiplexTilde::propSetValue);
}

void DemultiplexTilde::processBlock(const t_sample** in, t_sample** out)
{
    const size_t BS = blockSize();
    const size_t N = gain_.size();

    for (size_t i = 0; i < BS; i++) {
        t_sample v = in[0][i];
        for (size_t j = 0; j < N; j++)
            out[j][i] = gain_[j]() * v;
    }
}

void DemultiplexTilde::setupDSP(t_signal** in)
{
    SoundExternal::setupDSP(in);

    const double SR = samplerate();

    const size_t N = gain_.size();
    for (size_t i = 0; i < N; i++) {
        gain_[i].setDurationMs(20, SR);
    }
}

void DemultiplexTilde::onInlet(size_t /*n*/, const AtomList& lst)
{
    const int idx = lst.intAt(0, -1);
    const t_float fidx = lst.floatAt(0, -1);

    if (idx < 0 || idx >= gain_.size()) {
        OBJ_ERR << "invalid index: " << lst;
        return;
    }

    // pow xfade
    const t_float k = fidx - idx;
    const t_float k0 = 1 - k * k;
    const t_float k1 = 1 - (k - 1) * (k - 1);

    for (size_t i = 0; i < gain_.size(); i++) {
        if (i == idx)
            gain_[i].setTargetValue(k0);
        else if (i == idx + 1)
            gain_[i].setTargetValue(k1);
        else
            gain_[i].setTargetValue(0);
    }
}

void DemultiplexTilde::onList(const AtomList& lst)
{
    for (size_t i = 0; i < gain_.size(); i++)
        gain_[i].setTargetValue(clip<t_float>(lst.floatAt(i, 0), 0, 1));
}

AtomList DemultiplexTilde::propValue() const
{
    AtomList res;
    res.reserve(gain_.size());

    for (size_t i = 0; i < gain_.size(); i++)
        res.append(gain_[i].target());

    return res;
}

void DemultiplexTilde::propSetValue(const AtomList& lst)
{
    onList(lst);
}

void setup_flow_demultiplex_tilde()
{
    SoundExternalFactory<DemultiplexTilde> obj("flow.demultiplex~");
    obj.addAlias("flow.demux~");
    obj.addAlias("demux~");
}

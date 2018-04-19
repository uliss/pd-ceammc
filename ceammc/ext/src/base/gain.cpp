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
#include "gain.h"
#include "ceammc_convert.h"
#include "ceammc_factory.h"
#include "ceammc_signal.h"

static t_float toDb(t_float amp)
{
    return std::max<t_float>(-144, convert::amp2dbfs(amp));
}

static t_float fromDb(t_float db)
{
    if (db <= -144)
        return 0;

    return convert::dbfs2amp(db);
}

Gain::Gain(const PdArgs& args)
    : SoundExternal(args)
    , prev_bs_(0)
    , n_(std::max<int>(1, static_cast<int>(positionalFloatArgument(0, 1))))
{
    for (int i = 1; i < n_; i++) {
        createSignalInlet();
        createSignalOutlet();
    }

    createInlet();
    createSignalOutlet();

    allocateOutBlocks();
    gain_.assign(n_, 0);

    createCbProperty("@db", &Gain::propDb, &Gain::propSetDb);
    createCbProperty("@value", &Gain::propGain, &Gain::propSetGain);
}

void Gain::onInlet(size_t n, const AtomList& lst)
{
    if (!checkArgs(lst, ARG_FLOAT))
        return;

    t_float v = lst[0].asFloat();
    for (size_t i = 0; i < gain_.size(); i++)
        gain_[i].setTargetValue(v);
}

void Gain::onList(const AtomList& lst)
{
    const size_t N = std::min(lst.size(), gain_.size());
    for (size_t i = 0; i < N; i++)
        gain_[i].setTargetValue(lst[i].asFloat());
}

void Gain::processBlock(const t_sample** in, t_sample** out)
{
    const size_t BS = blockSize();
    const size_t N = n_;

    for (size_t i = 0; i < N; i++) {
        for (size_t j = 0; j < BS; j++) {
            outs_[i * BS + j] = gain_[i]() * in[i][j];
        }
    }

    for (size_t i = 0; i < N; i++) {
        for (size_t j = 0; j < BS; j++) {
            out[i][j] = outs_[i * BS + j];
        }
    }
}

void Gain::setupDSP(t_signal** sp)
{
    SoundExternal::setupDSP(sp);

    // block size changed...
    if (prev_bs_ != blockSize()) {
        allocateOutBlocks();
        prev_bs_ = blockSize();
    }
}

AtomList Gain::propDb() const
{
    return propGain().map(toDb);
}

AtomList Gain::propGain() const
{
    AtomList res;
    res.reserve(gain_.size());

    for (int i = 0; i < gain_.size(); i++)
        res.append(gain_[i].target());

    return res;
}

void Gain::propSetDb(const AtomList& lst)
{
    propSetGain(lst.map(fromDb));
}

void Gain::propSetGain(const AtomList& lst)
{
    const size_t N = std::min<size_t>(lst.size(), gain_.size());

    for (size_t i = 0; i < N; i++) {
        t_float v = std::max<t_float>(0, lst[i].asFloat());
        gain_[i].setTargetValue(v);
    }
}

void Gain::allocateOutBlocks()
{
    const size_t N = std::max<int>(1, n_) * blockSize();
    outs_.resize(N, 0);
}

void setup_gain_tilde()
{
    SoundExternalFactory<Gain> obj("gain~");
}

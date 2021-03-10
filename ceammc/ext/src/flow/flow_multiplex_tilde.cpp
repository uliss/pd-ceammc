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
#include "flow_multiplex_tilde.h"
#include "ceammc_convert.h"
#include "ceammc_factory.h"
#include "ceammc_format.h"
#include "fmt/format.h"

#include <array>

constexpr size_t DEF_INLETS = 2;
constexpr size_t MIN_NCHAN = 2;

static std::array<std::string, MAX_NCHAN> inlet_info = {};

static size_t chanMuliplier(const PdArgs& args)
{
    return (args.flags & MULTIPLEX_STEREO) ? 2 : 1;
}

MultiplexTilde::MultiplexTilde(const PdArgs& args)
    : SoundExternal(args)
{
    const size_t NCHAN = positionalConstant<DEF_INLETS, MIN_NCHAN, MAX_NCHAN>(0);
    for (size_t i = 1; i < NCHAN * chanMuliplier(args); i++)
        createSignalInlet();

    // control inlet
    createInlet();

    // out signals
    createSignalOutlet();
    if (args.flags == MULTIPLEX_STEREO)
        createSignalOutlet();

    gain_.assign(NCHAN, t_smooth(0));
    gain_[0].setTargetValue(1);

    createCbProperty("@value", &MultiplexTilde::propValue, &MultiplexTilde::propSetValue);
}

void MultiplexTilde::processBlock(const t_sample** in, t_sample** out)
{
    const size_t BS = blockSize();
    const size_t N = gain_.size();

    for (size_t i = 0; i < BS; i++) {
        t_sample v = 0;

        for (size_t j = 0; j < N; j++)
            v += gain_[j]() * in[j][i];

        out[0][i] = v;
    }
}

void MultiplexTilde::setupDSP(t_signal** in)
{
    SoundExternal::setupDSP(in);

    const double SR = samplerate();

    for (auto& g : gain_)
        g.setDurationMs(20, SR);
}

void MultiplexTilde::onInlet(size_t n, const AtomListView& lst)
{
    const int idx = lst.intAt(0, -1);
    const t_float fidx = lst.floatAt(0, -1);

    if (idx < 0 || idx >= gain_.size()) {
        OBJ_ERR << fmt::format(
            "expected input channel index in range [0-{}], got: {}",
            gain_.size() - 1, to_string(lst));

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

void MultiplexTilde::onList(const AtomList& lst)
{
    for (size_t i = 0; i < gain_.size(); i++)
        gain_[i].setTargetValue(clip<t_float>(lst[i].asFloat(), 0, 1));
}

AtomList MultiplexTilde::propValue() const
{
    AtomList res;
    res.reserve(gain_.size());

    for (auto& el : gain_)
        res.append(el.target());

    return res;
}

void MultiplexTilde::propSetValue(const AtomList& lst)
{
    onList(lst);
}

const char* MultiplexTilde::annotateInlet(size_t n) const
{
    if (n < gain_.size() && n < inlet_info.size())
        return inlet_info[n].c_str();
    else if (n == gain_.size())
        return "int: select input";
    else
        return nullptr;
}

void setup_flow_multiplex_tilde()
{
    for (size_t i = 0; i < inlet_info.size(); i++)
        inlet_info[i] = fmt::format("input\\[{}\\]", i);

    SoundExternalFactory<MultiplexTilde> obj("flow.multiplex~");
    obj.addAlias("flow.mux~");
    obj.addAlias("mux~");

    obj.addOutletInfo("signal: output");
}

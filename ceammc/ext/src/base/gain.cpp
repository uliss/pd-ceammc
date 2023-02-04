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
#include "ceammc_containers.h"
#include "ceammc_convert.h"
#include "ceammc_factory.h"
#include "ceammc_signal.h"
#include "fmt/core.h"

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

constexpr size_t DEFAULT_N = 1;
constexpr size_t MIN_N = 1;
constexpr size_t MAX_N = 64;

Gain::Gain(const PdArgs& args)
    : SoundExternal(args)
    , prev_bs_(0)
    , smooth_(nullptr)
{
    const size_t NCHAN(positionalConstant<DEFAULT_N, MIN_N, MAX_N>(0));

    info_in_.assign(NCHAN, {});
    info_out_.assign(NCHAN, {});
    info_in_[0] = fmt::format("in\\[{}\\]", 0);
    info_out_[0] = fmt::format("out\\[{}\\]", 0);
    for (size_t i = 1; i < NCHAN; i++) {
        createSignalInlet();
        createSignalOutlet();

        info_in_[i] = fmt::format("in\\[{}\\]", i);
        info_out_[i] = fmt::format("out\\[{}\\]", i);
    }

    createInlet();
    createSignalOutlet();

    allocateOutBlocks();
    gain_.assign(NCHAN, 0);

    createCbProperty("@db", &Gain::propDb, &Gain::propSetDb);
    createCbProperty("@value", &Gain::propGain, &Gain::propSetGain);

    smooth_ = new FloatProperty("@smooth_time", 20);
    smooth_->checkMin(1);
    smooth_->setUnitsMs();
    addProperty(smooth_);
}

void Gain::onInlet(size_t n, const AtomListView& lv)
{
    if (!checkArgs(lv, ARG_FLOAT))
        return;

    t_float v = lv[0].asFloat();
    for (size_t i = 0; i < gain_.size(); i++)
        gain_[i].setTargetValue(v);
}

void Gain::onList(const AtomListView& lv)
{
    const size_t N = std::min(lv.size(), gain_.size());
    for (size_t i = 0; i < N; i++)
        gain_[i].setTargetValue(lv[i].asFloat());
}

void Gain::processBlock(const t_sample** in, t_sample** out)
{
    const size_t BS = blockSize();
    const size_t N = gain_.size();

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

    // update smooth time
    const double ms = smooth_->value();
    const double SR = samplerate();
    for (auto& env : gain_)
        env.setDurationMs(ms, SR);
}

AtomList Gain::propDb() const
{
    AtomList res;
    propGain().view().mapFloat(toDb, res);
    return res;
}

AtomList Gain::propGain() const
{
    AtomList res;
    res.reserve(gain_.size());

    for (size_t i = 0; i < gain_.size(); i++)
        res.append(gain_[i].target());

    return res;
}

void Gain::propSetDb(const AtomListView& lv)
{
    SmallAtomList res;
    lv.mapFloat(fromDb, res);
    propSetGain(res.view());
}

void Gain::propSetGain(const AtomListView& lv)
{
    const size_t N = std::min<size_t>(lv.size(), gain_.size());

    for (size_t i = 0; i < N; i++) {
        t_float v = std::max<t_float>(0, lv[i].asFloat());
        gain_[i].setTargetValue(v);
    }
}

void Gain::m_plus(t_symbol* s, const AtomListView& lv)
{
    const size_t N = std::min<size_t>(gain_.size(), lv.size());

    for (size_t i = 0; i < N; i++) {
        t_float new_gain = gain_[i].target() + lv[i].asFloat();
        gain_[i].setTargetValue(std::max<t_float>(0, new_gain));
    }
}

static Atom negate(const Atom& a)
{
    return a.isFloat() ? -a.asFloat() : a;
}

void Gain::m_minus(t_symbol* s, const AtomListView& lv)
{
    SmallAtomList res;
    lv.map(negate, res);
    m_plus(s, res.view());
}

void Gain::m_plusDb(t_symbol* s, const AtomListView& lv)
{
    const size_t N = std::min<size_t>(gain_.size(), lv.size());

    for (size_t i = 0; i < N; i++) {
        t_float new_gain = gain_[i].target() * fromDb(lv[i].asFloat());
        gain_[i].setTargetValue(std::max<t_float>(0, new_gain));
    }
}

void Gain::m_minusDb(t_symbol* s, const AtomListView& lv)
{
    SmallAtomList res;
    lv.map(negate, res);
    m_plusDb(s, res.view());
}

void Gain::m_plusAll(t_symbol* s, const AtomListView& lv)
{
    t_float v = lv.floatAt(0, 0);
    for (size_t i = 0; i < gain_.size(); i++) {
        t_float new_gain = gain_[i].target() + v;
        gain_[i].setTargetValue(std::max<t_float>(0, new_gain));
    }
}

void Gain::m_minusAll(t_symbol* s, const AtomListView& lv)
{
    SmallAtomList res;
    lv.map(negate, res);
    m_plusAll(s, res.view());
}

void Gain::m_set(t_symbol* s, const AtomListView& lv)
{
    t_float v = lv.floatAt(0, 0);
    for (size_t i = 0; i < gain_.size(); i++)
        gain_[i].setTargetValue(std::max<t_float>(0, v));
}

void Gain::m_setDb(t_symbol* s, const AtomListView& lv)
{
    t_float v = lv.floatAt(0, 0);
    for (size_t i = 0; i < gain_.size(); i++)
        gain_[i].setTargetValue(std::max<t_float>(0, fromDb(v)));
}

void Gain::allocateOutBlocks()
{
    const size_t N = std::max<size_t>(1, gain_.size()) * blockSize();
    outs_.resize(N, 0);
}

const char* Gain::annotateInlet(size_t n) const
{
    if (n < info_in_.size())
        return info_in_[n].c_str();
    else
        return "float: set amplitue for all sources";
}

const char* Gain::annotateOutlet(size_t n) const
{
    if (n < info_out_.size())
        return info_out_[n].c_str();
    else
        return "float: set amplitue for all sources";
}

void setup_gain_tilde()
{
    SoundExternalFactory<Gain> obj("gain~");
    obj.addMethod("+", &Gain::m_plus);
    obj.addMethod("-", &Gain::m_minus);
    obj.addMethod("+db", &Gain::m_plusDb);
    obj.addMethod("-db", &Gain::m_minusDb);
    obj.addMethod("+all", &Gain::m_plusAll);
    obj.addMethod("-all", &Gain::m_minusAll);
    obj.addMethod("set", &Gain::m_set);
    obj.addMethod("set_db", &Gain::m_setDb);

    obj.setDescription("multislot signal gain");
    obj.addAuthor("Serge Poltavsky");
    obj.setKeywords({ "gain", "amplitude", "decibel" });
    obj.setCategory("base");
    obj.setSinceVersion(0, 6);
}

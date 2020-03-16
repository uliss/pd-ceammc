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
#include "synth_fgrain.h"
#include "ceammc_factory.h"
#include "ceammc_format.h"

#include "Granulate.h"

class Granulator : public stk::Granulate {
public:
    Granulator()
        : stk::Granulate()
    {
    }

    size_t voices() const { return grains_.size(); }
    size_t stretch() const { return std::min<size_t>(1000, gStretch_ + 1); }
    size_t grainDuration() const { return gDuration_; }
    size_t grainDelay() const { return gDelay_; }
    int grainOffset() const { return gOffset_; }
    t_float grainRamp() const { return gRampPercent_ / 100.f; }
    t_float grainRandomFactor() const { return gRandomFactor_ / 0.97; }

    void setGrainDuration(size_t d) { setGrainParameters(d, gRampPercent_, gOffset_, gDelay_); }
    void setGrainRamp(t_float p) { setGrainParameters(gDuration_, p * 100.f, gOffset_, gDelay_); }
    void setGrainOffset(int off) { setGrainParameters(gDuration_, gRampPercent_, off, gDelay_); }
    void setGrainDelay(size_t d) { setGrainParameters(gDuration_, gRampPercent_, gOffset_, d); }
};

SynthFGrain::SynthFGrain(const PdArgs& args)
    : SoundExternal(args)
    , synth_(new Granulator())
    , randomness_(0.1)
    , fname_(&s_)
{
    const size_t DEFAULT_STRETCH = 1;
    const size_t DEFAULT_VOICES = 1;

    {
        Property* p = createCbProperty("@file", &SynthFGrain::propFile, &SynthFGrain::propSetFile);
        p->info().setType(PropValueType::SYMBOL);
    }

    {
        Property* p = createCbProperty("@voices", &SynthFGrain::propVoices, &SynthFGrain::propSetVoices);
        p->info().setType(PropValueType::INTEGER);
        p->info().setRangeInt(0, 200);
        p->info().setConstraints(PropValueConstraints::CLOSED_RANGE);
        p->info().setDefault(DEFAULT_VOICES);
    }

    {
        Property* p = createCbProperty("@stretch", &SynthFGrain::propStretch, &SynthFGrain::propSetStretch);
        p->info().setType(PropValueType::INTEGER);
        p->info().setRangeInt(1, 1000);
        p->info().setConstraints(PropValueConstraints::CLOSED_RANGE);
        p->info().setDefault(DEFAULT_STRETCH);
    }

    {
        Property* p = createCbProperty("@random", &SynthFGrain::propRandom, &SynthFGrain::propSetRandom);
        p->info().setType(PropValueType::FLOAT);
        p->info().setRangeFloat(0, 0.97);
        p->info().setConstraints(PropValueConstraints::CLOSED_RANGE);
        p->info().setDefault(0.1f);
    }

    createCbProperty("@gramp", &SynthFGrain::propGrainRamp, &SynthFGrain::propSetGrainRamp);

    {
        Property* p = createCbProperty("@gdur", &SynthFGrain::propGrainDuration, &SynthFGrain::propSetGrainDuration);
        p->info().setType(PropValueType::INTEGER);
        p->info().setUnits(PropValueUnits::MSEC);
        p->info().setDefault(30);
        p->info().setMinInt(1);
        p->info().setConstraints(PropValueConstraints::GREATER_EQUAL);
    }

    {
        Property* p = createCbProperty("@goffset", &SynthFGrain::propGrainOffset, &SynthFGrain::propSetGrainOffset);
        p->info().setType(PropValueType::INTEGER);
        p->info().setDefault(int(0));
        p->info().setUnits(PropValueUnits::MSEC);
    }

    {
        Property* p = createCbProperty("@gdelay", &SynthFGrain::propGrainDelay, &SynthFGrain::propSetGrainDelay);
        p->info().setType(PropValueType::INTEGER);
        p->info().setDefault(int(0));
        p->info().setUnits(PropValueUnits::MSEC);
        p->info().setMinFloat(0);
    }

    synth_->setVoices(DEFAULT_VOICES);
    synth_->setStretch(DEFAULT_STRETCH);
    synth_->setRandomFactor(randomness_);

    createSignalOutlet();
}

SynthFGrain::~SynthFGrain()
{
    delete synth_;
}

void SynthFGrain::setupDSP(t_signal** sp)
{
    SoundExternal::setupDSP(sp);
    stk::Stk::setSampleRate(samplerate());
}

void SynthFGrain::processBlock(const t_sample**, t_sample** out)
{
    const size_t BS = blockSize();

    for (size_t i = 0; i < BS; i++)
        out[0][i] = synth_->tick();
}

AtomList SynthFGrain::propVoices() const
{
    return Atom(synth_->voices());
}

void SynthFGrain::propSetVoices(const AtomList& lst)
{
    if (!checkArgs(lst, ARG_NATURAL))
        return;

    size_t n = lst.floatAt(0, 1);
    if (n > 200) {
        OBJ_ERR << "too many voices: " << n;
        return;
    }

    synth_->setVoices(n);
}

AtomList SynthFGrain::propStretch() const
{
    return Atom(synth_->stretch());
}

void SynthFGrain::propSetStretch(const AtomList& lst)
{
    int stretch = lst.floatAt(0, 0);
    if (stretch < 1 || stretch > 1000) {
        OBJ_ERR << "invalid stretch value: " << stretch;
        OBJ_ERR << "should be in range: [1-1000]";
        return;
    }

    synth_->setStretch(stretch);
}

AtomList SynthFGrain::propRandom() const
{
    return Atom(synth_->grainRandomFactor());
}

void SynthFGrain::propSetRandom(const AtomList& lst)
{
    t_float k = lst.floatAt(0, 0);
    if (k < 0 || k > 1) {
        OBJ_ERR << "invalid grain random factor: " << k;
        OBJ_ERR << "should be in range [0-1]";
        return;
    }

    synth_->setRandomFactor(k);
}

AtomList SynthFGrain::propGrainDuration() const
{
    return Atom(synth_->grainDuration());
}

void SynthFGrain::propSetGrainDuration(const AtomList& lst)
{
    int dur = lst.floatAt(0, 0);
    if (dur < 1) {
        OBJ_ERR << "invalid grain duration: " << dur;
        OBJ_ERR << "should be >= 1";
        return;
    }

    synth_->setGrainDuration(dur);
}

AtomList SynthFGrain::propGrainOffset() const
{
    return Atom(synth_->grainOffset());
}

void SynthFGrain::propSetGrainOffset(const AtomList& lst)
{
    int ms = lst.floatAt(0, 0);
    synth_->setGrainOffset(ms);
}

AtomList SynthFGrain::propGrainDelay() const
{
    return Atom(synth_->grainDelay());
}

void SynthFGrain::propSetGrainDelay(const AtomList& lst)
{
    int ms = lst.floatAt(0, 0);
    if (ms < 0) {
        OBJ_ERR << "invalid grain delay: " << ms;
        OBJ_ERR << "should be >= 0";
        return;
    }

    synth_->setGrainDelay(ms);
}

AtomList SynthFGrain::propGrainRamp() const
{
    return Atom(synth_->grainRamp());
}

void SynthFGrain::propSetGrainRamp(const AtomList& lst)
{
    t_float perc = lst.floatAt(0, 0);
    if (perc < 0 || perc > 1) {
        OBJ_ERR << "invalid grain ramp: " << perc;
        OBJ_ERR << "should be in range [0-1]";
        return;
    }

    synth_->setGrainRamp(perc);
}

AtomList SynthFGrain::propFile() const
{
    return Atom(fname_);
}

void SynthFGrain::propSetFile(const AtomList& lst)
{
    std::string fname = to_string(lst);

    try {
        synth_->openFile(fname.c_str(), false);
    } catch (stk::StkError& e) {
        OBJ_ERR << "can't open file: " << fname;
        return;
    }

    fname_ = gensym(fname.c_str());
}

void SynthFGrain::m_open(t_symbol* s, const AtomList& lst)
{
    propSetFile(lst);
}

void SynthFGrain::m_reset(t_symbol* s, const AtomList& lst)
{
    synth_->reset();
}

void setup_synth_fgrain()
{
    SoundExternalFactory<SynthFGrain> obj("synth.fgrain~", OBJECT_FACTORY_DEFAULT);
    obj.addMethod("open", &SynthFGrain::m_open);
    obj.addMethod("reset", &SynthFGrain::m_reset);
}

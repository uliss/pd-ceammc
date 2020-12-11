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
    t_float grainRamp() const { return gRampPercent_; }
    t_float grainRandomFactor() const { return gRandomFactor_ / 0.97; }

    void setGrainDuration(size_t d) { setGrainParameters(d, gRampPercent_, gOffset_, gDelay_); }
    void setGrainRamp(int p) { setGrainParameters(gDuration_, p, gOffset_, gDelay_); }
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

    synth_->setVoices(DEFAULT_VOICES);
    synth_->setStretch(DEFAULT_STRETCH);
    synth_->setGrainDuration(30);
    synth_->setRandomFactor(randomness_);

    constexpr int MIN_VOICES = 0;
    constexpr int MAX_VOICES = 200;
    constexpr int MIN_STRETCH = 1;
    constexpr int MAX_STRETCH = 1000;
    constexpr t_float MIN_RANDOM = 0;
    constexpr t_float MAX_RANDOM = 0.97;

    createCbSymbolProperty(
        "@file",
        [this]() -> t_symbol* { return fname_; },
        [this](t_symbol* s) -> bool { return propSetFile(s); });

    createCbIntProperty(
        "@voices",
        [this]() -> int { return synth_->voices(); },
        [this](int n) -> bool { synth_->setVoices(n); return true; })
        ->setIntCheck(PropValueConstraints::CLOSED_RANGE, MIN_VOICES, MAX_VOICES);

    createCbIntProperty(
        "@stretch",
        [this]() -> int { return synth_->stretch(); },
        [this](int v) -> bool { synth_->setStretch(v); return true; })
        ->setIntCheck(PropValueConstraints::CLOSED_RANGE, MIN_STRETCH, MAX_STRETCH);

    createCbFloatProperty(
        "@random",
        [this]() -> t_float { return synth_->grainRandomFactor(); },
        [this](t_float f) -> bool { synth_->setRandomFactor(f); return true; })
        ->setFloatCheck(PropValueConstraints::CLOSED_RANGE, MIN_RANDOM, MAX_RANDOM);

    createCbIntProperty(
        "@gramp",
        [this]() -> int { return synth_->grainRamp(); },
        [this](int f) -> bool { synth_->setGrainRamp(f); return true; })
        ->setIntCheck(PropValueConstraints::CLOSED_RANGE, 1, 100);
    property("@gramp")->setUnits(PropValueUnits::PERCENT);

    Property* gdur = createCbIntProperty(
        "@gdur",
        [this]() -> int { return synth_->grainDuration(); },
        [this](int v) -> bool { synth_->setGrainDuration(v); return true; });
    gdur->setUnitsMs();
    gdur->setIntCheck(PropValueConstraints::GREATER_EQUAL, 1);

    Property* goffset = createCbIntProperty(
        "@goffset",
        [this]() -> int { return synth_->grainOffset(); },
        [this](int v) -> bool { synth_->setGrainOffset(v); return true; });
    goffset->setUnitsMs();
    goffset->checkNonNegative();

    Property* delay = createCbIntProperty(
        "@gdelay",
        [this]() -> int { return synth_->grainDelay(); },
        [this](int v) -> bool { synth_->setGrainDelay(v); return true; });
    delay->setUnitsMs();
    delay->checkNonNegative();

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

bool SynthFGrain::propSetFile(t_symbol* fname)
{
    try {
        synth_->openFile(fname->s_name, false);
    } catch (stk::StkError& e) {
        OBJ_ERR << "can't open file: " << fname << ", error: " << e.getMessage();
        return false;
    }

    fname_ = fname;
    return true;
}

void SynthFGrain::m_open(t_symbol* s, const AtomListView& lst)
{
    propSetFile(lst.toT<t_symbol*>(&s_));
}

void SynthFGrain::m_reset(t_symbol* s, const AtomListView&)
{
    synth_->reset();
}

void setup_synth_fgrain()
{
    SoundExternalFactory<SynthFGrain> obj("synth.fgrain~", OBJECT_FACTORY_DEFAULT);
    obj.addMethod("open", &SynthFGrain::m_open);
    obj.addMethod("reset", &SynthFGrain::m_reset);
}

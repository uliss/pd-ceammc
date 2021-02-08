#include "synth_glass_harm.h"
#include "ceammc_clock.h"
#include "ceammc_factory.h"
#include "synth_faust_with_freq.h"

using namespace ceammc;
using SynthGlass = SynthWithFreq<faust_synth_glass_harm_tilde>;

class SynthGlassHarm : public SynthGlass {
    ClockLambdaFunction clock_;

public:
    SynthGlassHarm(const PdArgs& args)
        : SynthGlass(args)
        , clock_([this]() { gate_->setValue(0); })
    {
    }

    void onBang() override
    {
        gate_->setValue(1);
        dsp_->instanceClear();
        clock_.delay(150);
    }

    void m_reset(t_symbol*, const AtomListView&)
    {
        gate_->setValue(0);
        dsp_->instanceClear();
    }
};

void setup_synth_glass_harm_tilde()
{
    SynthGlassHarm::initSymTab();

    SoundExternalFactory<SynthGlassHarm> obj("synth.glass_harm~", OBJECT_FACTORY_DEFAULT);
    obj.addMethod("reset", &SynthGlassHarm::m_reset);
    obj.addMethod("note", &SynthGlassHarm::m_note);
}

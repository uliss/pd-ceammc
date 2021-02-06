#include "synth_glass_harm.h"
#include "ceammc_clock.h"
#include "ceammc_factory.h"
#include "synth_faust_with_freq.h"

using namespace ceammc;
using SynthMarimba = SynthWithFreq<faust_synth_glass_harm_tilde>;

class SynthGlassHarm : public SynthMarimba {
    UIProperty* gate_;
    ClockLambdaFunction clock_;

public:
    SynthGlassHarm(const PdArgs& args)
        : SynthMarimba(args)
        , gate_(static_cast<UIProperty*>(property(PROP_GATE)))
        , clock_([this]() { gate_->setValue(0); })
    {
    }

    void onBang() override
    {
        gate_->setValue(1);
        clock_.delay(150);
    }
};

void setup_synth_glass_harm_tilde()
{
    SynthGlassHarm::initSymTab();

    SoundExternalFactory<SynthGlassHarm> obj("synth.glass_harm~", OBJECT_FACTORY_DEFAULT);
    obj.addMethod("reset", &SynthGlassHarm::m_reset);
    obj.addMethod("note", &SynthMarimba::m_note);
}

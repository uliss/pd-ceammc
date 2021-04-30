#include "synth_standard_bell.h"
#include "ceammc_clock.h"
#include "ceammc_factory.h"

using namespace ceammc;

class SynthStandardBell : public faust_synth_standard_bell_tilde {
    UIProperty* gate_;
    ClockLambdaFunction clock_;

public:
    SynthStandardBell(const PdArgs& args)
        : faust_synth_standard_bell_tilde(args)
        , gate_(static_cast<UIProperty*>(property(gensym("@gate"))))
        , clock_([this]() { gate_->setValue(0); })
    {
    }

    void onBang() override
    {
        gate_->setValue(1);
        clock_.delay(200);
    }
};

void setup_synth_standard_bell_tilde()
{
    SoundExternalFactory<SynthStandardBell> obj("synth.standard_bell~", OBJECT_FACTORY_DEFAULT);
    obj.addMethod("reset", &SynthStandardBell::m_reset);
}

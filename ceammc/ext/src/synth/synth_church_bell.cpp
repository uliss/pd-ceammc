#include "synth_church_bell.h"
#include "ceammc_clock.h"
#include "ceammc_factory.h"

using namespace ceammc;

class SynthChurchBell : public faust_synth_church_bell_tilde {
    UIProperty* gate_;
    ClockLambdaFunction clock_;

public:
    SynthChurchBell(const PdArgs& args)
        : faust_synth_church_bell_tilde(args)
        , gate_(static_cast<UIProperty*>(property(gensym("@gate"))))
        , clock_([this]() { gate_->setValue(0); })
    {
    }

    void onBang() override
    {
        gate_->setValue(1);
        clock_.delay(150);
    }
};

void setup_synth_church_bell_tilde()
{
    SoundExternalFactory<SynthChurchBell> obj("synth.church_bell~", OBJECT_FACTORY_DEFAULT);
    obj.addMethod("reset", &SynthChurchBell::m_reset);

    obj.setDescription("generic church bell modal model");
    obj.setCategory("synth");
    obj.setKeywords({"bell", "church"});
}

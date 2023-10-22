#include "synth_french_bell.h"
#include "ceammc_clock.h"
#include "ceammc_factory.h"

using namespace ceammc;

class SynthFrenchBell : public faust_synth_french_bell_tilde {
    UIProperty* gate_;
    ClockLambdaFunction clock_;

public:
    SynthFrenchBell(const PdArgs& args)
        : faust_synth_french_bell_tilde(args)
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

void setup_synth_french_bell_tilde()
{
    SoundExternalFactory<SynthFrenchBell> obj("synth.french_bell~", OBJECT_FACTORY_DEFAULT);
    obj.addMethod("reset", &SynthFrenchBell::m_reset);

    obj.setDescription("french church bell modal model");
    obj.setCategory("synth");
    obj.setKeywords({"bell", "church", "french"});
}

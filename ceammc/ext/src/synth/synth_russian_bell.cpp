#include "synth_russian_bell.h"
#include "ceammc_clock.h"
#include "ceammc_factory.h"

using namespace ceammc;

class SynthRussianBell : public faust_synth_russian_bell_tilde {
    UIProperty* gate_;
    ClockLambdaFunction clock_;

public:
    SynthRussianBell(const PdArgs& args)
        : faust_synth_russian_bell_tilde(args)
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

void setup_synth_russian_bell_tilde()
{
    SoundExternalFactory<SynthRussianBell> obj("synth.russian_bell~", OBJECT_FACTORY_DEFAULT);
    obj.addMethod("reset", &SynthRussianBell::m_reset);

    obj.setDescription("russian church bell modal model");
    obj.setCategory("synth");
    obj.setKeywords({"bell", "church", "russian"});
}

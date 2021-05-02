#include "synth_snare.h"
#include "ceammc_clock.h"
#include "ceammc_factory.h"

using namespace ceammc;

class SynthSnare : public faust_synth_snare_tilde {
    UIProperty* gate_;
    UIProperty* attack_;
    UIProperty* decay_;
    UIProperty* release_;
    ClockLambdaFunction clock_;

public:
    SynthSnare(const PdArgs& args)
        : faust_synth_snare_tilde(args)
        , gate_(findProperty("@gate"))
        , attack_(findProperty("@attack"))
        , decay_(findProperty("@decay"))
        , release_(findProperty("@release"))
        , clock_([this]() { gate_->setValue(0); })
    {
    }

    void onBang() override
    {
        if (gate_->value())
            return;

        gate_->setValue(1);
        clock_.delay(std::ceil(envLength() + 2));
    }

private:
    UIProperty* findProperty(const char* name)
    {
        return static_cast<UIProperty*>(property(gensym(name)));
    }

    t_float envLength() const
    {
        return attack_->value() + decay_->value() + release_->value();
    }
};

void setup_synth_snare_tilde()
{
    SoundExternalFactory<SynthSnare> obj("synth.snare~", OBJECT_FACTORY_DEFAULT);
    obj.addMethod("reset", &SynthSnare::m_reset);
}

#include "synth_birds.h"
#include "ceammc_clock.h"
#include "ceammc_factory.h"

using namespace ceammc;

static t_symbol* PROP_GATE;

class SynthBirds : public faust_synth_birds_tilde {
    UIProperty* gate_;
    ClockLambdaFunction clock_;

public:
    SynthBirds(const PdArgs& args)
        : faust_synth_birds_tilde(args)
        , gate_(static_cast<UIProperty*>(property(PROP_GATE)))
        , clock_([this]() { gate_->setValue(0); })
    {
        bindPositionalArgsToProps({ gensym("@speed"), gensym("@probability") });
    }

    void onBang() override
    {
        gate_->setValue(1);
        clock_.delay(10);
    }
};

void setup_synth_birds_tilde()
{
    PROP_GATE = gensym("@gate");

    SoundExternalFactory<SynthBirds> obj("synth.birds~", OBJECT_FACTORY_DEFAULT);
}

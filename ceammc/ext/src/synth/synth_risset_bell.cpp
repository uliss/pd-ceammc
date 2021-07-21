#include "synth_risset_bell.h"
#include "ceammc_clock.h"
#include "ceammc_factory.h"

using namespace ceammc;

constexpr int GATE_DUR = 50;

class SynthRissetBell : public faust_synth_risset_bell_tilde {
    UIProperty* gate_;
    ClockLambdaFunction clock_;

public:
    SynthRissetBell(const PdArgs& args)
        : faust_synth_risset_bell_tilde(args)
        , gate_(static_cast<UIProperty*>(property(gensym("@gate"))))
        , clock_([this]() { gate_->setValue(0); })
    {
        bindPositionalArgsToProps({ gensym("@freq"), gensym("@dur") });
    }

    void onBang() override
    {
        gate_->setValue(1);
        clock_.delay(GATE_DUR);
    }

    void onFloat(t_float f) override
    {
        if (f > 0 && f <= 1) {
            gate_->setValue(f);
            clock_.delay(GATE_DUR);
        }
    }

    void onList(const AtomList& lst) override
    {
        t_float gain = 1;
        t_float dur = 4;

        if (lst.size() == 2 && lst[0].isFloat() && lst[1].isFloat()) {
            gain = lst[0].asT<t_float>();
            dur = lst[1].asT<t_float>();
        } else if (lst.size() == 1 && lst[0].isFloat()) {
            gain = lst[0].asT<t_float>();
        } else {
            OBJ_ERR << "usage: GAIN(0..1] DUR?";
            return;
        }

        if (gain <= 0 || gain > 1) {
            OBJ_ERR << "expected gain value in (0..1] range, got: " << gain;
            return;
        }

        if (dur < 0.1 || dur > 120) {
            OBJ_ERR << "expected duration in seconds in [0.1..120] range, got: " << dur;
            return;
        }

        gate_->setValue(gain);
        clock_.delay(GATE_DUR);
    }
};

void setup_synth_risset_bell_tilde()
{
    SoundExternalFactory<SynthRissetBell> obj("synth.risset_bell~", OBJECT_FACTORY_DEFAULT);
    obj.addMethod("reset", &SynthRissetBell::m_reset);
}

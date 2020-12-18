#include "env_smooth.h"
#include "ceammc_clock.h"
#include "ceammc_factory.h"
#include "env_faust_play.h"

using namespace ceammc;

static t_symbol* PROP_GATE;
static t_symbol* PROP_DURATION;

class EnvSmooth : public faust_env_smooth_tilde {
    ClockLambdaFunction done_;

    UIProperty* prop_duration_;
    UIProperty* prop_gate_;

public:
    EnvSmooth(const PdArgs& args)
        : faust_env_smooth_tilde(args)
        , done_([this]() { bangTo(1); })
        , prop_duration_((UIProperty*)property(PROP_DURATION))
        , prop_gate_((UIProperty*)property(PROP_GATE))
    {
        bindPositionalArgsToProps({ PROP_DURATION });
        createOutlet();
    }

    bool processAnyProps(t_symbol* sel, const AtomListView& lst) override
    {
        if (sel == PROP_GATE && !lst.boolAt(0, false)) {
            done_.delay(prop_duration_->value());
        }

        return faust_env_smooth_tilde::processAnyProps(sel, lst);
    }

    void m_reset(t_symbol*, const AtomListView&)
    {
        prop_gate_->setValue(0);
        dsp_->instanceClear();
        resetClocks();
    }

private:
    void resetClocks()
    {
        done_.unset();
    }
};

void setup_env_smooth_tilde()
{
    PROP_GATE = gensym("@gate");
    PROP_DURATION = gensym("@duration");

    using EnvSmooth2 = EnvAutoplay<EnvSmooth>;
    SoundExternalFactory<EnvSmooth2> obj("env.smooth~");
    obj.addMethod("play", &EnvSmooth2::m_play);
    obj.addMethod("reset", &EnvSmooth2::m_reset);
    obj.setXletsInfo({ "input signal" }, { "enveloped signal", "bang when done" });
}

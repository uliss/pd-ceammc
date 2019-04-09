#include "env_smooth.h"
#include "ceammc_clock.h"
#include "ceammc_factory.h"
#include "env_faust_play.h"

using namespace ceammc;

class EnvSmooth : public faust_env_smooth_tilde {
    ClockMemberFunction<EnvSmooth> done_;

    UIProperty* prop_duration_;
    UIProperty* prop_gate_;

public:
    EnvSmooth(const PdArgs& args)
        : faust_env_smooth_tilde(args)
        , done_(this, &EnvSmooth::done)
        , prop_duration_((UIProperty*)property(gensym("@duration")))
        , prop_gate_((UIProperty*)property(gensym("@gate")))
    {
        bindPositionalArgsToProps({ gensym("@duration") });
        createOutlet();
    }

    bool processAnyProps(t_symbol* sel, const AtomList& lst) override
    {
        if (sel == gensym("@gate") && !atomlistToValue<bool>(lst, false)) {
            done_.delay(prop_duration_->value());
        }

        return faust_env_smooth_tilde::processAnyProps(sel, lst);
    }

    void m_reset(t_symbol*, const AtomList&)
    {
        prop_gate_->setValue(0);
        dsp_->instanceClear();
        resetClocks();
    }

private:
    void done()
    {
        bangTo(1);
    }

    void resetClocks()
    {
        done_.unset();
    }
};

void setup_env_smooth_tilde()
{
    typedef EnvAutoplay<EnvSmooth> EnvSmooth2;
    SoundExternalFactory<EnvSmooth2> obj("env.smooth~");
    obj.addMethod("reset", &EnvSmooth2::m_reset);
}

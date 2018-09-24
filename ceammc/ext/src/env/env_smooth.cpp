#include "env_smooth.h"
#include "ceammc_clock.h"
#include "ceammc_factory.h"

static t_symbol* SYM_PROP_DURATION = gensym("@duration");
static t_symbol* SYM_PROP_GATE = gensym("@gate");

using namespace ceammc;

class EnvSmooth : public faust_env_smooth_tilde {
    ClockMemberFunction<EnvSmooth> done_;

    UIProperty* prop_duration_;
    UIProperty* prop_gate_;

public:
    EnvSmooth(const PdArgs& args)
        : faust_env_smooth_tilde(args)
        , done_(this, &EnvSmooth::done)
        , prop_duration_((UIProperty*)property(SYM_PROP_DURATION))
        , prop_gate_((UIProperty*)property(SYM_PROP_GATE))
    {
        bindPositionalArgsToProps({ SYM_PROP_DURATION });
        createOutlet();
    }

    bool processAnyProps(t_symbol* sel, const AtomList& lst) override
    {
        if (sel == SYM_PROP_GATE && !atomlistToValue<bool>(lst, false)) {
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
    SoundExternalFactory<EnvSmooth> obj("env.smooth~");
    obj.addMethod("reset", &EnvSmooth::m_reset);
}

#include "fx_sdelay.h"
#include "ceammc_factory.h"

using namespace ceammc;

static t_symbol* SYM_PROP_DELAY = gensym("@delay");
static t_symbol* SYM_PROP_FEEDBACK = gensym("@feedback");

class FxSmoothDelay : public faust_fx_sdelay_tilde {
public:
    FxSmoothDelay(const PdArgs& args)
        : faust_fx_sdelay_tilde(args)
    {
        bindPositionalArgsToProps({ SYM_PROP_DELAY, SYM_PROP_FEEDBACK });
    }

    void m_clear(t_symbol*, const AtomList&)
    {
        dsp_->instanceClear();
    }
};

void setup_fx_sdelay_tilde()
{
    SoundExternalFactory<FxSmoothDelay> obj("fx.sdelay~");
    obj.addMethod("clear", &FxSmoothDelay::m_clear);
    obj.addMethod("reset", &FxSmoothDelay::m_clear);
}

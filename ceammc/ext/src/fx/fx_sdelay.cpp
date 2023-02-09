#include "fx_sdelay.h"
#include "ceammc_factory.h"

using namespace ceammc;

class FxSmoothDelay : public faust_fx_sdelay_tilde {
public:
    FxSmoothDelay(const PdArgs& args)
        : faust_fx_sdelay_tilde(args)
    {
        static t_symbol* SYM_PROP_DELAY = gensym("@delay");
        static t_symbol* SYM_PROP_FEEDBACK = gensym("@feedback");

        bindPositionalArgsToProps({ SYM_PROP_DELAY, SYM_PROP_FEEDBACK });
    }
};

void setup_fx_sdelay_tilde()
{
    SoundExternalFactory<FxSmoothDelay> obj("fx.sdelay~");
    obj.addMethod("clear", &FxSmoothDelay::m_reset);
    obj.addMethod("reset", &FxSmoothDelay::m_reset);

    obj.setDescription("smooth delay with a feedback control");
    obj.setCategory("fx");
    obj.setKeywords({"fx", "echo", "delay"});
}

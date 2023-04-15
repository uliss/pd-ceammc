#include "fx_sdelay.h"
#include "ceammc_faust_factory.h"

using namespace ceammc;

class FxSmoothDelay : public faust_fx_sdelay_tilde {
public:
    FxSmoothDelay(const PdArgs& args)
        : faust_fx_sdelay_tilde(args)
    {
        bindPositionalArgsToProps({ gensym("@delay"), gensym("@feedback") });
    }
};

void setup_fx_sdelay_tilde()
{
    FaustFactory<FxSmoothDelay> obj("fx.sdelay~");
    obj.addMethod("clear", &FxSmoothDelay::m_reset);

    obj.setDescription("smooth delay with a feedback control");
    obj.setCategory("fx");
    obj.setKeywords({"fx", "echo", "delay"});
}

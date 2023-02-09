#include "fx_secho.h"
#include "ceammc_factory.h"

using namespace ceammc;

class FxSEcho : public faust_fx_secho_tilde {
public:
    FxSEcho(const PdArgs& args)
        : faust_fx_secho_tilde(args)
    {
        static t_symbol* SYM_PROP_DELAY = gensym("@delay");
        static t_symbol* SYM_PROP_FEEDBACK = gensym("@feedback");

        bindPositionalArgsToProps({ SYM_PROP_DELAY, SYM_PROP_FEEDBACK });
    }
};

void setup_fx_secho_tilde()
{
    SoundExternalFactory<FxSEcho> obj("fx.secho~");
    obj.addMethod("reset", &FxSEcho::m_reset);

    obj.setDescription("enchanced echo effect that does not click on delay change");
    obj.setCategory("fx");
    obj.setKeywords({"fx", "echo", "delay"});
}

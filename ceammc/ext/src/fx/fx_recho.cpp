#include "fx_recho.h"
#include "ceammc_factory.h"

using namespace ceammc;

class FxREcho : public faust_fx_recho_tilde {
public:
    FxREcho(const PdArgs& args)
        : faust_fx_recho_tilde(args)
    {
        static t_symbol* SYM_PROP_DELAY = gensym("@delay");
        static t_symbol* SYM_PROP_FEEDBACK = gensym("@feedback");

        bindPositionalArgsToProps({ SYM_PROP_DELAY, SYM_PROP_FEEDBACK });
    }
};

void setup_fx_recho_tilde()
{
    SoundExternalFactory<FxREcho> obj("fx.recho~");
    obj.addMethod("reset", &FxREcho::m_reset);

    obj.setDescription("reversed echo effect");
    obj.setCategory("fx");
    obj.setKeywords({"fx", "echo", "reversed"});
}

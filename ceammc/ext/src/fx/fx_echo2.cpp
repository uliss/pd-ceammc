#include "fx_echo2.h"
#include "ceammc_factory.h"

using namespace ceammc;

class FxEcho2 : public faust_fx_echo2_tilde {
public:
    FxEcho2(const PdArgs& args)
        : faust_fx_echo2_tilde(args)
    {
        static t_symbol* SYM_PROP_DELAY = gensym("@delay");
        static t_symbol* SYM_PROP_FEEDBACK = gensym("@feedback");

        bindPositionalArgsToProps({ SYM_PROP_DELAY, SYM_PROP_FEEDBACK });
    }
};

void setup_fx_echo2_tilde()
{
    SoundExternalFactory<FxEcho2> obj("fx.echo2~");
    obj.addMethod("reset", &FxEcho2::m_reset);
}

#include "fx_echo.h"
#include "ceammc_factory.h"

using namespace ceammc;

class FxEcho : public faust_fx_echo_tilde {
public:
    FxEcho(const PdArgs& args)
        : faust_fx_echo_tilde(args)
    {
        static t_symbol* SYM_PROP_DELAY = gensym("@delay");
        static t_symbol* SYM_PROP_FEEDBACK = gensym("@feedback");

        bindPositionalArgsToProps({ SYM_PROP_DELAY, SYM_PROP_FEEDBACK });
    }
};

void setup_fx_echo_tilde()
{
    SoundExternalFactory<FxEcho> obj("fx.echo~");
    obj.addMethod("reset", &FxEcho::m_reset);
}

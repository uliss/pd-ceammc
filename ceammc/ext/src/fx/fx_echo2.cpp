#include "fx_echo2.h"
#include "ceammc_faust_factory.h"

using namespace ceammc;

class FxEcho2 : public faust_fx_echo2_tilde {
public:
    FxEcho2(const PdArgs& args)
        : faust_fx_echo2_tilde(args)
    {
        auto SYM_PROP_DELAY = gensym("@delay");
        auto SYM_PROP_FEEDBACK = gensym("@feedback");

        bindPositionalArgsToProps({ SYM_PROP_DELAY, SYM_PROP_FEEDBACK });
    }
};

void setup_fx_echo2_tilde()
{
    FaustFactory<FxEcho2> obj("fx.echo2~");

    obj.setDescription("stereo echo effect");
    obj.setCategory("fx");
    obj.setKeywords({ "fx", "echo", "stereo", "delay" });
}

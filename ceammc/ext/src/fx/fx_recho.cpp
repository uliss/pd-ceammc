#include "fx_recho.h"
#include "ceammc_faust_factory.h"

using namespace ceammc;

class FxREcho : public faust_fx_recho_tilde {
public:
    FxREcho(const PdArgs& args)
        : faust_fx_recho_tilde(args)
    {
        bindPositionalArgsToProps({ gensym("@delay"), gensym("@feedback") });
    }
};

void setup_fx_recho_tilde()
{
    FaustFactory<FxREcho> obj("fx.recho~");

    obj.setDescription("reversed echo effect");
    obj.setCategory("fx");
    obj.setKeywords({"fx", "echo", "reversed"});
}

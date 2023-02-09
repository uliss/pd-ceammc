#include "dyn_limit.h"
#include "ceammc_factory.h"
using namespace ceammc;

class DynLimit : public faust_dyn_limit_tilde {
public:
    DynLimit(const PdArgs& args)
        : faust_dyn_limit_tilde(args)
    {
    }
};

void setup_dyn_limit_tilde()
{
    SoundExternalFactory<DynLimit> obj("dyn.limit~");

    obj.setDescription("mono 1176LN Peak Limiter");
    obj.setCategory("dyn");
    obj.setKeywords({"limiter"});
}

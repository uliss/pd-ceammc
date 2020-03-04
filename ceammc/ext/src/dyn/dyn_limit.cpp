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
}

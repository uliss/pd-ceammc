#include "dyn_limit2.h"
#include "ceammc_factory.h"
using namespace ceammc;

class DynLimit2 : public faust_dyn_limit2_tilde {
public:
    DynLimit2(const PdArgs& args)
        : faust_dyn_limit2_tilde(args)
    {
    }
};

void setup_dyn_limit2_tilde()
{
    SoundExternalFactory<DynLimit2> obj("dyn.limit2~");
}

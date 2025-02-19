#include "dyn_limit2.h"
#include "ceammc_faust_factory.h"
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
    FaustFactory<DynLimit2> obj("dyn.limit2~");

    obj.setDescription("stereo 1176LN Peak Limiter");
    obj.setCategory("dyn");
    obj.setKeywords({ "limiter" });
}

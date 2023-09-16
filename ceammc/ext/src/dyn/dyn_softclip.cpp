#include "dyn_softclip.h"
#include "ceammc_faust_factory.h"
using namespace ceammc;

class DynSoftClip : public faust_dyn_softclip_tilde {
public:
    DynSoftClip(const PdArgs& args)
        : faust_dyn_softclip_tilde(args)
    {
    }
};

void setup_dyn_softclip_tilde()
{
    FaustFactory<DynSoftClip> obj("dyn.softclip~");

    obj.setDescription("soft signal clip");
    obj.setCategory("dyn");
    obj.setKeywords({ "dyn", "clip", "limiter" });
}

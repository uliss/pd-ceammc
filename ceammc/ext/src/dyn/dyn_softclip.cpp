#include "dyn_softclip.h"
#include "ceammc_factory.h"
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
    SoundExternalFactory<DynSoftClip> obj("dyn.softclip~");
}

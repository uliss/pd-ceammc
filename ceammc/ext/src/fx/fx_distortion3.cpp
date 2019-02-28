#include "fx_distortion3.h"
#include "ceammc_factory.h"

using namespace ceammc;

class FxDistortion3 : public faust_fx_distortion3_tilde {
public:
    FxDistortion3(const PdArgs& args)
        : faust_fx_distortion3_tilde(args)
    {
    }
};

void setup_fx_distortion3_tilde()
{
    SoundExternalFactory<FxDistortion3> obj("fx.distortion3~");
}

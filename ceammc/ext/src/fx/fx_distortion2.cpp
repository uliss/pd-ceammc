#include "fx_distortion2.h"
#include "ceammc_factory.h"

using namespace ceammc;

class FxDistortion2 : public faust_fx_distortion2_tilde {
public:
    FxDistortion2(const PdArgs& args)
        : faust_fx_distortion2_tilde(args)
    {
    }
};

void setup_fx_distortion2_tilde()
{
    SoundExternalFactory<FxDistortion2> obj("fx.distortion2~");

    obj.setDescription("distortion #2 from Guitarix effects set");
    obj.setCategory("fx");
    obj.setKeywords({"fx", "distortion"});
}

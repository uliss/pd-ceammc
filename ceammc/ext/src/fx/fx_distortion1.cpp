#include "fx_distortion1.h"
#include "ceammc_factory.h"

using namespace ceammc;

class FxDistortion1 : public faust_fx_distortion1_tilde {
public:
    FxDistortion1(const PdArgs& args)
        : faust_fx_distortion1_tilde(args)
    {
    }
};

void setup_fx_distortion1_tilde()
{
    SoundExternalFactory<FxDistortion1> obj("fx.distortion1~");

    obj.setDescription("distortion #1 from Guitarix effects set");
    obj.setCategory("fx");
    obj.setKeywords({"fx", "distortion"});
}

#include "fx_chorus.h"
#include "ceammc_faust_factory.h"

using namespace ceammc;

class FxChorus : public faust_fx_chorus_tilde {
public:
    FxChorus(const PdArgs& args)
        : faust_fx_chorus_tilde(args)
    {
    }
};

void setup_fx_chorus_tilde()
{
    FaustFactory<FxChorus> obj("fx.chorus~");

    obj.setDescription("Chorus effect");
    obj.setCategory("fx");
    obj.setKeywords({ "fx", "chorus" });
}

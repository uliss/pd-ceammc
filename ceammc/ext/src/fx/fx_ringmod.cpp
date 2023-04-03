#include "fx_ringmod.h"
#include "ceammc_factory.h"

using namespace ceammc;

class FxRingMod : public faust_fx_ringmod_tilde {
public:
    FxRingMod(const PdArgs& args)
        : faust_fx_ringmod_tilde(args)
    {
        bindPositionalArgsToProps({ gensym("@drywet") });
    }
};

void setup_fx_ringmod_tilde()
{
    SoundExternalFactory<FxRingMod> obj("fx.ringmod~");
    obj.addMethod("reset", &FxRingMod::m_reset);

    obj.setDescription("simple ring modulation effect");
    obj.setCategory("fx");
    obj.setKeywords({ "fx", "ring", "modulation" });
}

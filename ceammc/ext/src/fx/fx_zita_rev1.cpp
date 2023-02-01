#include "fx_zita_rev1.h"
#include "ceammc_factory.h"

using namespace ceammc;

class FxZitaRev1 : public faust_fx_zita_rev1_tilde {
public:
    FxZitaRev1(const PdArgs& args)
        : faust_fx_zita_rev1_tilde(args)
    {
        bindPositionalArgsToProps({ gensym("@decay_low"), gensym("@decay_mid"), gensym("@drywet") });
    }
};

void setup_fx_zita_rev1_tilde()
{
    SoundExternalFactory<FxZitaRev1> obj("fx.zita_rev1~");
    obj.addMethod("reset", &FxZitaRev1::m_reset);
}

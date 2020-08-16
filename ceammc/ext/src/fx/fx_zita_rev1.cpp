#include "fx_zita_rev1.h"
#include "ceammc_factory.h"

using namespace ceammc;

class FxZitaRev1 : public faust_fx_zita_rev1_tilde {
public:
    FxZitaRev1(const PdArgs& args)
        : faust_fx_zita_rev1_tilde(args)
    {
        static t_symbol* SYM_PROP_DECAY_LOW = gensym("@decay_low");
        static t_symbol* SYM_PROP_DECAY_MID = gensym("@decay_mid");

        bindPositionalArgsToProps({ SYM_PROP_DECAY_LOW, SYM_PROP_DECAY_MID });
    }
};

void setup_fx_zita_rev1_tilde()
{
    SoundExternalFactory<FxZitaRev1> obj("fx.zita_rev1~");
    obj.addMethod("reset", &FxZitaRev1::m_reset);
}

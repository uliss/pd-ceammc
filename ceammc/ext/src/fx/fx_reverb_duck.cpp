#include "fx_reverb_duck.h"
#include "ceammc_factory.h"

using namespace ceammc;

class FxReverbDuck : public faust_fx_reverb_duck_tilde {
public:
    FxReverbDuck(const PdArgs& args)
        : faust_fx_reverb_duck_tilde(args)
    {
        static t_symbol* SYM_PROP_DECAY_LOW = gensym("@decay_low");
        static t_symbol* SYM_PROP_DECAY_MID = gensym("@decay_mid");

        bindPositionalArgsToProps({ SYM_PROP_DECAY_LOW, SYM_PROP_DECAY_MID });
    }
};

void setup_fx_reverb_duck_tilde()
{
    SoundExternalFactory<FxReverbDuck> obj("fx.reverb_duck~");
    obj.addMethod("reset", &FxReverbDuck::m_reset);
}

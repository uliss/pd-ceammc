#include "noise_crackle.h"
#include "ceammc_factory.h"

using namespace ceammc;

static t_symbol* SYM_PROP_FREQ = gensym("@freq");

class NoiseCrackle : public faust_noise_crackle_tilde {
public:
    NoiseCrackle(const PdArgs& args)
        : faust_noise_crackle_tilde(args)
    {
        bindPositionalArgsToProps({ SYM_PROP_FREQ });
    }
};

void setup_noise_crackle_tilde()
{
    SoundExternalFactory<NoiseCrackle> obj("noise.crackle~");
}

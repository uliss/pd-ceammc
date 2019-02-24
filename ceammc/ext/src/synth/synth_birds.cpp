#include "synth_birds.h"
#include "ceammc_factory.h"

using namespace ceammc;

static t_symbol* SYM_PROP_SPEED = gensym("@speed");
static t_symbol* SYM_PROP_PROBABILITY = gensym("@probability");

class SynthBirds : public faust_synth_birds_tilde {
public:
    SynthBirds(const PdArgs& args)
        : faust_synth_birds_tilde(args)
    {
        bindPositionalArgsToProps({ SYM_PROP_SPEED, SYM_PROP_PROBABILITY });
    }
};

void setup_synth_birds_tilde()
{
    SoundExternalFactory<SynthBirds> obj("synth.birds~");
}

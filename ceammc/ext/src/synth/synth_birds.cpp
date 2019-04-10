#include "synth_birds.h"
#include "ceammc_factory.h"

using namespace ceammc;

class SynthBirds : public faust_synth_birds_tilde {
public:
    SynthBirds(const PdArgs& args)
        : faust_synth_birds_tilde(args)
    {
        bindPositionalArgsToProps({ gensym("@speed"), gensym("@probability") });
    }
};

void setup_synth_birds_tilde()
{
    SoundExternalFactory<SynthBirds> obj("synth.birds~", OBJECT_FACTORY_DEFAULT);
}

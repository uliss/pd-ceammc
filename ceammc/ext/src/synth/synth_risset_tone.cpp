#include "synth_risset_tone.h"
#include "ceammc_factory.h"

using namespace ceammc;

void setup_synth_risset_tone_tilde()
{
    SoundExternalFactory<faust_synth_risset_tone_tilde> obj("synth.risset_tone~", OBJECT_FACTORY_DEFAULT);
}

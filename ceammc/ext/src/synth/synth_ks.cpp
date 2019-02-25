#include "synth_ks.h"
#include "ceammc_factory.h"

using namespace ceammc;

void setup_synth_ks_tilde()
{
    SoundExternalFactory<faust_synth_ks_tilde> obj("synth.ks~", OBJECT_FACTORY_DEFAULT);
}

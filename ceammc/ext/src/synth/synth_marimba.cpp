#include "synth_marimba.h"
#include "ceammc_factory.h"

using namespace ceammc;

void setup_synth_marimba_tilde()
{
    SoundExternalFactory<faust_synth_marimba_tilde> obj("synth.marimba~", OBJECT_FACTORY_DEFAULT);
}

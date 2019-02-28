#include "synth_church_bell.h"
#include "ceammc_factory.h"

using namespace ceammc;

void setup_synth_church_bell_tilde()
{
    SoundExternalFactory<faust_synth_church_bell_tilde> obj("synth.church_bell~", OBJECT_FACTORY_DEFAULT);
}

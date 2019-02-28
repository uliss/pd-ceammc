#include "synth_dx7.h"
#include "ceammc_factory.h"

using namespace ceammc;

void setup_synth_dx7_tilde()
{
    SoundExternalFactory<faust_synth_dx7_tilde> obj("synth.dx7~", OBJECT_FACTORY_DEFAULT);
}

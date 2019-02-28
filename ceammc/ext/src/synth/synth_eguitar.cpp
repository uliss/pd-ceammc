#include "synth_eguitar.h"
#include "ceammc_factory.h"

using namespace ceammc;

void setup_synth_eguitar_tilde()
{
    SoundExternalFactory<faust_synth_eguitar_tilde> obj("synth.eguitar~", OBJECT_FACTORY_DEFAULT);
}

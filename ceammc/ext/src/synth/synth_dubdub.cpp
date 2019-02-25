#include "synth_dubdub.h"
#include "ceammc_factory.h"

using namespace ceammc;

void setup_synth_dubdub_tilde()
{
    SoundExternalFactory<faust_synth_dubdub_tilde> obj("synth.dubdub~", OBJECT_FACTORY_DEFAULT);
}

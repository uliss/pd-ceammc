#include "fx_vocoder.h"
#include "ceammc_factory.h"

using namespace ceammc;

void setup_fx_vocoder_tilde()
{
    SoundExternalFactory<faust_fx_vocoder_tilde> obj("fx.vocoder~");
}

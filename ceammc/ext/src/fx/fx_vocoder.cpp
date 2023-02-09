#include "fx_vocoder.h"
#include "ceammc_factory.h"

using namespace ceammc;

void setup_fx_vocoder_tilde()
{
    SoundExternalFactory<faust_fx_vocoder_tilde> obj("fx.vocoder~");

    obj.setDescription("very simple vocoder where the spectrum of the modulation signal is analyzed using a 32-band filter bank");
    obj.setCategory("fx");
    obj.setKeywords({"vocoder"});
}

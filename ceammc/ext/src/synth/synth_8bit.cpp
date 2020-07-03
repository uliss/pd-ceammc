#include "synth_8bit.h"
#include "ceammc_factory.h"

Synth8Bit::Synth8Bit(const PdArgs& args)
    : SoundExternal(args)
    , t_(0)
{
    createSignalOutlet();
}

void Synth8Bit::processBlock(const t_sample** in, t_sample** out)
{
    size_t bs = blockSize();

    for (size_t i = 0; i < bs; i++) {
        char b = t_ * ((t_ >> 12 | t_ >> 8) & 63 & (t_ >> 4));
        out[0][i] = b / 127.f;

        if (i % 5 == 0)
            t_++;
    }
}

void setup_fx_8bit()
{
    SoundExternalFactory<Synth8Bit> obj("synth.8bit~", OBJECT_FACTORY_DEFAULT);
    obj.useDefaultPdFloatFn();
}

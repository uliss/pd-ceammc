#include "ceammc_convert.h"
#include "ceammc_factory.h"
#include "pan_linsig.h"

PanLinSig::PanLinSig(const PdArgs& args)
    : SoundExternal(args)
{
    createSignalInlet();
    createSignalOutlet();
    createSignalOutlet();
}

void PanLinSig::processBlock(const t_sample** in, t_sample** out)
{
    const size_t bs = blockSize();

    for (size_t i = 0; i < bs; i++) {
        auto r = (in[1][i] + 1) * 0.5;
        auto l = 1 - r;
        auto v = in[0][i];

        out[0][i] = v * l;
        out[1][i] = v * r;
    }
}

void setup_pan_linsig()
{
    SoundExternalFactory<PanLinSig> obj("pan.linsig~");
}

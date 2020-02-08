#include <cmath>

#include "pan_cosine.h"
#include "ceammc_factory.h"

PanCosine::PanCosine(const PdArgs& args)
    : PanBase(args)
{
}

void PanCosine::processBlock(const t_sample** in, t_sample** out)
{
    const size_t bs = blockSize();
    const t_float v = (pos_->value() * 0.5 + 0.5) * M_PI_2;

    for (size_t i = 0; i < bs; i += 8) {
        const t_float sv = smooth_pos_.get(v);
        const t_float l = cosf(sv);
        const t_float r = sinf(sv);

        t_float v0 = in[0][i + 0];
        t_float v1 = in[0][i + 1];
        t_float v2 = in[0][i + 2];
        t_float v3 = in[0][i + 3];
        t_float v4 = in[0][i + 4];
        t_float v5 = in[0][i + 5];
        t_float v6 = in[0][i + 6];
        t_float v7 = in[0][i + 7];

        out[0][i + 0] = v0 * l;
        out[0][i + 1] = v1 * l;
        out[0][i + 2] = v2 * l;
        out[0][i + 3] = v3 * l;
        out[0][i + 4] = v4 * l;
        out[0][i + 5] = v5 * l;
        out[0][i + 6] = v6 * l;
        out[0][i + 7] = v7 * l;

        out[1][i + 0] = v0 * r;
        out[1][i + 1] = v1 * r;
        out[1][i + 2] = v2 * r;
        out[1][i + 3] = v3 * r;
        out[1][i + 4] = v4 * r;
        out[1][i + 5] = v5 * r;
        out[1][i + 6] = v6 * r;
        out[1][i + 7] = v7 * r;
    }
}

void setup_pan_cosine()
{
    SoundExternalFactory<PanCosine> obj("pan.cos~");
}

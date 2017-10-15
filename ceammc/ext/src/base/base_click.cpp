#include "base_click.h"
#include "ceammc_factory.h"

Click::Click(const PdArgs& a)
    : SoundExternal(a)
    , do_click_(false)
{
    createSignalOutlet();
}

void Click::onBang()
{
    do_click_ = true;
}

void Click::processBlock(const t_sample** in, t_sample** out)
{
    const size_t BS = blockSize();

    for (size_t i = 0; i < BS; i += 8) {
        out[0][i] = 0;
        out[0][i + 1] = 0;
        out[0][i + 2] = 0;
        out[0][i + 3] = 0;
        out[0][i + 4] = 0;
        out[0][i + 5] = 0;
        out[0][i + 6] = 0;
        out[0][i + 7] = 0;
    }

    if (do_click_) {
        out[0][0] = 1;
        do_click_ = false;
    }
}

extern "C" void click_tilde_setup()
{
    SoundExternalFactory<Click> obj("click~", OBJECT_FACTORY_DEFAULT | OBJECT_FACTORY_NO_FLOAT);
}

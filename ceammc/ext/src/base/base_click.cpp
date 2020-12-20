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

void click_tilde_setup()
{
    SoundExternalFactory<Click> obj("click~", OBJECT_FACTORY_DEFAULT);
    obj.useDefaultPdFloatFn();
    obj.useDefaultPdListFn();

    obj.setDescription("Output single impulse on bang");
    obj.addAuthor("Serge Poltavsky");
    obj.setKeywords({ "click" });
    obj.setCategory("misc");
    obj.setSinceVersion(0, 4);

    obj.setXletsInfo({ "bang: trigger output" }, { "signal: single impulse" });
}

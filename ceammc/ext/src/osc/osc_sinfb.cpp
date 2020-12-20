#include "osc_sinfb.h"
#include "ceammc_factory.h"

using namespace ceammc;

class OscSinFb : public faust_osc_sinfb_tilde {
public:
    OscSinFb(const PdArgs& args)
        : faust_osc_sinfb_tilde(args)
    {
        createInlet();
        setInitSignalValue(parsedPosArgs().floatAt(0, 0));
        bindPositionalArgToProperty(1, gensym("@feedback"));
    }

    void onInlet(size_t n, const AtomList&) override
    {
        dsp_->instanceClear();
    }
};

void setup_osc_sinfb_tilde()
{
    SoundExternalFactory<OscSinFb> obj("osc.sinfb~");
}

#include "osc_saw4.h"
#include "ceammc_factory.h"

using namespace ceammc;

class OscSaw4 : public faust_osc_saw4_tilde {
public:
    OscSaw4(const PdArgs& args)
        : faust_osc_saw4_tilde(args)
    {
        createInlet();
        setInitSignalValue(parsedPosArgs().floatAt(0, 0));
    }

    void onInlet(size_t n, const AtomList&) override
    {
        dsp_->instanceClear();
    }
};

void setup_osc_saw4_tilde()
{
    SoundExternalFactory<OscSaw4> obj("osc.saw4~");
}

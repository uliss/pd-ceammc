#include "osc_saw.h"
#include "ceammc_factory.h"

using namespace ceammc;

class OscSaw : public faust_osc_saw_tilde {
public:
    OscSaw(const PdArgs& args)
        : faust_osc_saw_tilde(args)
    {
        createInlet();
        setInitSignalValue(parsedPosArgs().floatAt(0, 0));
    }

    void onInlet(size_t n, const AtomListView&) override
    {
        dsp_->instanceClear();
    }
};

void setup_osc_saw_tilde()
{
    SoundExternalFactory<OscSaw> obj("osc.saw~");
}

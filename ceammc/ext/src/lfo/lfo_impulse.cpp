#include "lfo_impulse.h"
#include "ceammc_factory.h"

using namespace ceammc;

class LfoImpulse : public faust_lfo_impulse_tilde {
public:
    LfoImpulse(const PdArgs& args)
        : faust_lfo_impulse_tilde(args)
    {
        createInlet();
        setInitSignalValue(parsedPosArgs().floatAt(0, 0));
    }

    void onInlet(size_t n, const AtomListView&) override
    {
        dsp_->instanceClear();
    }
};

void setup_lfo_impulse_tilde()
{
    SoundExternalFactory<LfoImpulse> obj("lfo.impulse~");
}

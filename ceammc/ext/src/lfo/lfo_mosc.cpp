#include "lfo_mosc.h"
#include "ceammc_factory.h"
using namespace ceammc;

class LfoMosc : public faust_lfo_mosc_tilde {
public:
    LfoMosc(const PdArgs& args)
        : faust_lfo_mosc_tilde(args)
    {
        createInlet();
        setInitSignalValue(parsedPosArgs().floatAt(0, 0));
    }

    void onInlet(size_t n, const AtomListView&) override
    {
        dsp_->instanceClear();
    }
};

void setup_lfo_mosc_tilde()
{
    SoundExternalFactory<LfoMosc> obj("lfo.mosc~");
    obj.addMethod("reset", &LfoMosc::m_reset);
    obj.setXletsInfo({ "freq in hz", "bang: reset phase" }, { "oscillator output" });
}

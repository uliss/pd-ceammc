#include "lfo_pulse.h"
#include "ceammc_factory.h"
using namespace ceammc;

class LfoPulse : public faust_lfo_pulse_tilde {
public:
    LfoPulse(const PdArgs& args)
        : faust_lfo_pulse_tilde(args)
    {
        static t_symbol* SYM_PROP_DUTY = gensym("@duty");

        createInlet();
        setInitSignalValue(parsedPosArgs().floatAt(0, 0));
        bindPositionalArgToProperty(1, SYM_PROP_DUTY);
    }

    void onInlet(size_t n, const AtomListView&) override
    {
        dsp_->instanceClear();
    }
};

void setup_lfo_pulse_tilde()
{
    SoundExternalFactory<LfoPulse> obj("lfo.pulse~");
    obj.addMethod("reset", &LfoPulse::m_reset);
    obj.setXletsInfo({ "freq in hz", "bang: reset phase" }, { "pulse wave in \\[-1,+1\\] range" });
}

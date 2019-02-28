#include "lfo_pulse.h"
#include "ceammc_factory.h"
using namespace ceammc;

static t_symbol* SYM_PROP_DUTY = gensym("@duty");

class LfoPulse : public faust_lfo_pulse_tilde {
public:
    LfoPulse(const PdArgs& args)
        : faust_lfo_pulse_tilde(args)
    {
        createInlet();
        setInitSignalValue(positionalFloatArgument(0, 0));
        bindPositionalArgToProperty(1, SYM_PROP_DUTY);
    }

    void onInlet(size_t n, const AtomList&) override
    {
        dsp_->instanceClear();
    }
};

void setup_lfo_pulse_tilde()
{
    SoundExternalFactory<LfoPulse> obj("lfo.pulse~");
}

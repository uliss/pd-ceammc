#include "lfo_pulse_pos.h"
#include "ceammc_factory.h"
using namespace ceammc;

class LfoPulsePos : public faust_lfo_pulse_pos_tilde {
public:
    LfoPulsePos(const PdArgs& args)
        : faust_lfo_pulse_pos_tilde(args)
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

void setup_lfo_pulse_pos_tilde()
{
    SoundExternalFactory<LfoPulsePos> obj("lfo.+pulse~");
}

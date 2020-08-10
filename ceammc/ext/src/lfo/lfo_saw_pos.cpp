#include "lfo_saw_pos.h"
#include "ceammc_factory.h"
using namespace ceammc;

class LfoSawPos : public faust_lfo_saw_pos_tilde {
public:
    LfoSawPos(const PdArgs& args)
        : faust_lfo_saw_pos_tilde(args)
    {
        static t_symbol* SYM_PROP_INVERT = gensym("@invert");

        createInlet();
        setInitSignalValue(parsedPosArgs().floatAt(0, 0));
        bindPositionalArgToProperty(1, SYM_PROP_INVERT);
    }

    void onInlet(size_t n, const AtomList&) override
    {
        dsp_->instanceClear();
    }
};

void setup_lfo_saw_pos_tilde()
{
    SoundExternalFactory<LfoSawPos> obj("lfo.+saw~");
}

#include "lfo_saw.h"
#include "ceammc_factory.h"
using namespace ceammc;

static t_symbol* SYM_PROP_INVERT = gensym("@invert");

class LfoSaw : public faust_lfo_saw_tilde {
public:
    LfoSaw(const PdArgs& args)
        : faust_lfo_saw_tilde(args)
    {
        createInlet();
        setInitSignalValue(positionalFloatArgument(0, 0));
        bindPositionalArgToProperty(1, SYM_PROP_INVERT);
    }

    void onInlet(size_t n, const AtomList&) override
    {
        dsp_->instanceClear();
    }
};

void setup_lfo_saw_tilde()
{
    SoundExternalFactory<LfoSaw> obj("lfo.saw~");
}

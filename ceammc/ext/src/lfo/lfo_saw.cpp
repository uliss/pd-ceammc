#include "lfo_saw.h"
#include "ceammc_factory.h"
using namespace ceammc;

class LfoSaw : public faust_lfo_saw_tilde {
public:
    LfoSaw(const PdArgs& args)
        : faust_lfo_saw_tilde(args)
    {
        static t_symbol* SYM_PROP_INVERT = gensym("@invert");

        createInlet();
        setInitSignalValue(parsedPosArgs().floatAt(0, 0));
        bindPositionalArgToProperty(1, SYM_PROP_INVERT);
    }

    void onInlet(size_t n, const AtomListView&) override
    {
        dsp_->instanceClear();
    }
};

void setup_lfo_saw_tilde()
{
    SoundExternalFactory<LfoSaw> obj("lfo.saw~");
}

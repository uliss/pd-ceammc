#include "fx_freqshift.h"
#include "ceammc_factory.h"

using namespace ceammc;

class FxFreqShift : public faust_fx_freqshift_tilde {
public:
    FxFreqShift(const PdArgs& args)
        : faust_fx_freqshift_tilde(args)
    {
        static t_symbol* SYM_PROP_DELAY = gensym("@delay");
        static t_symbol* SYM_PROP_FEEDBACK = gensym("@feedback");

        bindPositionalArgsToProps({ SYM_PROP_DELAY, SYM_PROP_FEEDBACK });
    }
};

void setup_fx_freqshift_tilde()
{
    SoundExternalFactory<FxFreqShift> obj("fx.freqshift~");
}

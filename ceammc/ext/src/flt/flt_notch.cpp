#include "flt_notch.h"
#include "ceammc_factory.h"

using namespace ceammc;

static t_symbol* SYM_PROP_FREQ = gensym("@freq");
static t_symbol* SYM_PROP_WIDTH = gensym("@width");

class FltNotch : public faust_flt_notch_tilde {
public:
    FltNotch(const PdArgs& args)
        : faust_flt_notch_tilde(args)
    {
        bindPositionalArgsToProps({ SYM_PROP_FREQ, SYM_PROP_WIDTH });
    }
};

void setup_flt_notch_tilde()
{
    SoundExternalFactory<FltNotch> obj("flt.notch~");
}

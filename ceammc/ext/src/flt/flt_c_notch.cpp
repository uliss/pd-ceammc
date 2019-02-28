#include "flt_c_notch.h"
#include "ceammc_factory.h"

using namespace ceammc;

static t_symbol* SYM_PROP_FREQ = gensym("@freq");
static t_symbol* SYM_PROP_Q = gensym("@q");

class FltCNotch : public faust_flt_c_notch_tilde {
public:
    FltCNotch(const PdArgs& args)
        : faust_flt_c_notch_tilde(args)
    {
        bindPositionalArgsToProps({ SYM_PROP_FREQ, SYM_PROP_Q });
    }
};

void setup_flt_c_notch_tilde()
{
    SoundExternalFactory<FltCNotch> obj("flt.c_notch~", OBJECT_FACTORY_DEFAULT);
}

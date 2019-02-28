#include "flt_c_lpf.h"
#include "ceammc_factory.h"

using namespace ceammc;

static t_symbol* SYM_PROP_FREQ = gensym("@freq");
static t_symbol* SYM_PROP_Q = gensym("@q");

class FltCLpf : public faust_flt_c_lpf_tilde {
public:
    FltCLpf(const PdArgs& args)
        : faust_flt_c_lpf_tilde(args)
    {
        bindPositionalArgsToProps({ SYM_PROP_FREQ, SYM_PROP_Q });
    }
};

void setup_flt_c_lpf_tilde()
{
    SoundExternalFactory<FltCLpf> obj("flt.c_lpf~", OBJECT_FACTORY_DEFAULT);
}

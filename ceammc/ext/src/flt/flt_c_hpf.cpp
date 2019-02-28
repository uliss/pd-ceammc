#include "flt_c_hpf.h"
#include "ceammc_factory.h"

using namespace ceammc;

static t_symbol* SYM_PROP_FREQ = gensym("@freq");
static t_symbol* SYM_PROP_Q = gensym("@q");

class FltCHpf : public faust_flt_c_hpf_tilde {
public:
    FltCHpf(const PdArgs& args)
        : faust_flt_c_hpf_tilde(args)
    {
        bindPositionalArgsToProps({ SYM_PROP_FREQ, SYM_PROP_Q });
    }
};

void setup_flt_c_hpf_tilde()
{
    SoundExternalFactory<FltCHpf> obj("flt.c_hpf~", OBJECT_FACTORY_DEFAULT);
}

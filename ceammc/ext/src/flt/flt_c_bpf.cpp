#include "flt_c_bpf.h"
#include "ceammc_factory.h"

using namespace ceammc;

static t_symbol* SYM_PROP_FREQ = gensym("freq");
static t_symbol* SYM_PROP_Q = gensym("q");

class FltCBpf : public faust_flt_c_bpf_tilde {
public:
    FltCBpf(const PdArgs& args)
        : faust_flt_c_bpf_tilde(args)
    {
        bindPositionalArgsToProps({ SYM_PROP_FREQ, SYM_PROP_Q });
    }
};

void setup_flt_c_bpf_tilde()
{
    SoundExternalFactory<FltCBpf> obj("flt.c_bpf~");
}

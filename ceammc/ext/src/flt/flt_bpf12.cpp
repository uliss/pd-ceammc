#include "flt_bpf12.h"
#include "ceammc_factory.h"

using namespace ceammc;

static t_symbol* SYM_PROP_FREQ = gensym("freq");
static t_symbol* SYM_PROP_Q = gensym("q");

class FltBpf12 : public faust_flt_bpf12_tilde {
public:
    FltBpf12(const PdArgs& args)
        : faust_flt_bpf12_tilde(args)
    {
        bindPositionalArgsToProps({ SYM_PROP_FREQ, SYM_PROP_Q });
    }
};

void setup_flt_bpf12_tilde()
{
    SoundExternalFactory<FltBpf12> obj("flt.bpf12~");
}

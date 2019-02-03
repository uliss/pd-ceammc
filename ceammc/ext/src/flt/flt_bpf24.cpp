#include "flt_bpf24.h"
#include "ceammc_factory.h"

#include "ceammc_factory.h"

using namespace ceammc;

static t_symbol* SYM_PROP_FREQ = gensym("freq");
static t_symbol* SYM_PROP_Q = gensym("q");

class FltBpf24 : public faust_flt_bpf24_tilde {
public:
    FltBpf24(const PdArgs& args)
        : faust_flt_bpf24_tilde(args)
    {
        bindPositionalArgsToProps({ SYM_PROP_FREQ, SYM_PROP_Q });
    }
};

void setup_flt_bpf24_tilde()
{
    SoundExternalFactory<FltBpf24> obj("flt.bpf24~");
}


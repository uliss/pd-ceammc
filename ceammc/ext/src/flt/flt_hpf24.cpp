#include "flt_hpf24.h"
#include "ceammc_factory.h"

using namespace ceammc;

static t_symbol* SYM_PROP_FREQ = gensym("@freq");

class FltHpf24 : public faust_flt_hpf24_tilde {
public:
    FltHpf24(const PdArgs& args)
        : faust_flt_hpf24_tilde(args)
    {
        bindPositionalArgsToProps({ SYM_PROP_FREQ });
    }
};

void setup_flt_hpf24_tilde()
{
    SoundExternalFactory<FltHpf24> obj("flt.hpf24~");
}

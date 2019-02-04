#include "flt_hpf12.h"
#include "ceammc_factory.h"

#include "ceammc_factory.h"

using namespace ceammc;

static t_symbol* SYM_PROP_FREQ = gensym("@freq");

class FltHpf12 : public faust_flt_hpf12_tilde {
public:
    FltHpf12(const PdArgs& args)
        : faust_flt_hpf12_tilde(args)
    {
        bindPositionalArgsToProps({ SYM_PROP_FREQ });
    }
};

void setup_flt_hpf12_tilde()
{
    SoundExternalFactory<FltHpf12> obj("flt.hpf12~");
}

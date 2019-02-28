#include "flt_lpf12.h"
#include "ceammc_factory.h"

using namespace ceammc;

static t_symbol* SYM_PROP_FREQ = gensym("@freq");

class FltLpf12 : public faust_flt_lpf12_tilde {
public:
    FltLpf12(const PdArgs& args)
        : faust_flt_lpf12_tilde(args)
    {
        bindPositionalArgsToProps({ SYM_PROP_FREQ });
    }
};

void setup_flt_lpf12_tilde()
{
    SoundExternalFactory<FltLpf12> obj("flt.lpf12~");
}

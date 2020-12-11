#include "flt_bpf24.h"
#include "ceammc_factory.h"

using namespace ceammc;

class FltBpf24 : public faust_flt_bpf24_tilde {
public:
    FltBpf24(const PdArgs& args)
        : faust_flt_bpf24_tilde(args)
    {
        bindPositionalArgsToProps({ gensym("@freq"), gensym("@q") });
    }
};

void setup_flt_bpf24_tilde()
{
    SoundExternalFactory<FltBpf24> obj("flt.bpf24~");
    obj.addMethod("reset", &FltBpf24::m_reset);
}

#include "flt_c_lpf.h"
#include "ceammc_factory.h"

using namespace ceammc;

class FltCLpf : public faust_flt_c_lpf_tilde {
public:
    FltCLpf(const PdArgs& args)
        : faust_flt_c_lpf_tilde(args)
    {
        bindPositionalArgsToProps({ gensym("@freq"), gensym("@q") });
    }
};

void setup_flt_c_lpf_tilde()
{
    SoundExternalFactory<FltCLpf> obj("flt.c_lpf~", OBJECT_FACTORY_DEFAULT);
}

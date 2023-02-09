#include "flt_c_bpf.h"
#include "ceammc_factory.h"

using namespace ceammc;

class FltCBpf : public faust_flt_c_bpf_tilde {
public:
    FltCBpf(const PdArgs& args)
        : faust_flt_c_bpf_tilde(args)
    {
        bindPositionalArgsToProps({ gensym("@freq"), gensym("@q") });
    }
};

void setup_flt_c_bpf_tilde()
{
    SoundExternalFactory<FltCBpf> obj("flt.c_bpf~", OBJECT_FACTORY_DEFAULT);

    obj.setDescription("BPF coefficient calculator for biquad filter");
    obj.setCategory("flt");
    obj.setKeywords({"filter", "biquad", "bpf"});
}

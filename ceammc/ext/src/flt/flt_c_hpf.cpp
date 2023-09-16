#include "flt_c_hpf.h"
#include "ceammc_factory.h"

using namespace ceammc;

class FltCHpf : public faust_flt_c_hpf_tilde {
public:
    FltCHpf(const PdArgs& args)
        : faust_flt_c_hpf_tilde(args)
    {
        bindPositionalArgsToProps({ gensym("@freq"), gensym("@q") });
    }
};

void setup_flt_c_hpf_tilde()
{
    SoundExternalFactory<FltCHpf> obj("flt.c_hpf~", OBJECT_FACTORY_DEFAULT);

    obj.setDescription("HPF coefficient calculator for biquad filter");
    obj.setCategory("flt");
    obj.setKeywords({"filter", "biquad", "highpass"});
}

#include "flt_bpf12.h"
#include "ceammc_factory.h"

using namespace ceammc;

class FltBpf12 : public faust_flt_bpf12_tilde {
public:
    FltBpf12(const PdArgs& args)
        : faust_flt_bpf12_tilde(args)
    {
        bindPositionalArgsToProps({ gensym("@freq"), gensym("@q") });
    }
};

void setup_flt_bpf12_tilde()
{
    SoundExternalFactory<FltBpf12> obj("flt.bpf12~");
    obj.addMethod("reset", &FltBpf12::m_reset);

    obj.setDescription("Two pole band-pass butterworth filter");
    obj.setCategory("flt");
    obj.setKeywords({"filter", "biquad", "bpf"});
}

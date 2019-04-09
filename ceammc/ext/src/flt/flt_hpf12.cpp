#include "flt_hpf12.h"
#include "ceammc_factory.h"

using namespace ceammc;

class FltHpf12 : public faust_flt_hpf12_tilde {
public:
    FltHpf12(const PdArgs& args)
        : faust_flt_hpf12_tilde(args)
    {
        bindPositionalArgsToProps({ gensym("@freq") });
    }
};

void setup_flt_hpf12_tilde()
{
    SoundExternalFactory<FltHpf12> obj("flt.hpf12~");
}

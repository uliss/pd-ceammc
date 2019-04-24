#include "flt_lpf24.h"
#include "ceammc_factory.h"

using namespace ceammc;

class FltLpf24 : public faust_flt_lpf24_tilde {
public:
    FltLpf24(const PdArgs& args)
        : faust_flt_lpf24_tilde(args)
    {
        bindPositionalArgsToProps({ gensym("@freq") });
    }
};

void setup_flt_lpf24_tilde()
{
    SoundExternalFactory<FltLpf24> obj("flt.lpf24~");
}

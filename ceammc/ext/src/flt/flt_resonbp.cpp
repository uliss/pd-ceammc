#include "flt_resonbp.h"
#include "ceammc_factory.h"

using namespace ceammc;

class FltResonBp : public faust_flt_resonbp_tilde {
public:
    FltResonBp(const PdArgs& args)
        : faust_flt_resonbp_tilde(args)
    {
        static t_symbol* SYM_PROP_FREQ = gensym("@freq");
        static t_symbol* SYM_PROP_Q = gensym("@q");

        bindPositionalArgsToProps({ SYM_PROP_FREQ, SYM_PROP_Q });
    }
};

void setup_flt_resonbp_tilde()
{
    SoundExternalFactory<FltResonBp> obj("flt.resonbp~");
}

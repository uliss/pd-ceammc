#include "flt_eq_peak_cq.h"
#include "ceammc_factory.h"

using namespace ceammc;

class FxEqPeakCq : public faust_flt_eq_peak_cq_tilde {
public:
    FxEqPeakCq(const PdArgs& args)
        : faust_flt_eq_peak_cq_tilde(args)
    {
        static t_symbol* SYM_PROP_FREQ = gensym("@freq");
        static t_symbol* SYM_PROP_GAIN = gensym("@gain");
        static t_symbol* SYM_PROP_Q = gensym("@q");

        bindPositionalArgsToProps({ SYM_PROP_FREQ, SYM_PROP_GAIN, SYM_PROP_Q });
    }
};

void setup_flt_eq_peak_cq_tilde()
{
    SoundExternalFactory<FxEqPeakCq> obj("flt.eq_peak_cq~");

    obj.setDescription("constant-Q second order peaking equalizer section");
    obj.setCategory("flt");
    obj.setKeywords({"filter", "equalizer"});
}

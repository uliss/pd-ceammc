#include "flt_eq_peak.h"
#include "ceammc_factory.h"

using namespace ceammc;

class FxEqPeak : public faust_flt_eq_peak_tilde {
public:
    FxEqPeak(const PdArgs& args)
        : faust_flt_eq_peak_tilde(args)
    {
        static t_symbol* SYM_PROP_FREQ = gensym("@freq");
        static t_symbol* SYM_PROP_GAIN = gensym("@gain");
        static t_symbol* SYM_PROP_BANDWIDTH = gensym("@bandwidth");

        bindPositionalArgsToProps({ SYM_PROP_FREQ, SYM_PROP_GAIN, SYM_PROP_BANDWIDTH });
    }
};

void setup_flt_eq_peak_tilde()
{
    SoundExternalFactory<FxEqPeak> obj("flt.eq_peak~");

    obj.setDescription("second order \"peaking equalizer\" section (gain boost or cut near some frequency). Also called a \"parametric equalizer\" section.");
    obj.setCategory("flt");
    obj.setKeywords({"filter", "equalizer"});
}

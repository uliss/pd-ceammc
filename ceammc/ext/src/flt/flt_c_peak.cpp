#include "flt_c_peak.h"
#include "ceammc_factory.h"

using namespace ceammc;

class FltCPeak : public faust_flt_c_peak_tilde {
public:
    FltCPeak(const PdArgs& args)
        : faust_flt_c_peak_tilde(args)
    {
        bindPositionalArgsToProps({ gensym("@freq"), gensym("@gain"), gensym("@q") });
    }
};

void setup_flt_c_peak_tilde()
{
    SoundExternalFactory<FltCPeak> obj("flt.c_peak~", OBJECT_FACTORY_DEFAULT);

    obj.setDescription("Peaking equalizer calculator for biquad");
    obj.setCategory("flt");
    obj.setKeywords({"filter", "equalizer"});
}

#include "flt_c_peak.h"
#include "ceammc_factory.h"

using namespace ceammc;

static t_symbol* SYM_PROP_FREQ = gensym("@freq");
static t_symbol* SYM_PROP_GAIN = gensym("@gain");
static t_symbol* SYM_PROP_Q = gensym("@q");

class FltCPeak : public faust_flt_c_peak_tilde {
public:
    FltCPeak(const PdArgs& args)
        : faust_flt_c_peak_tilde(args)
    {
        bindPositionalArgsToProps({ SYM_PROP_FREQ, SYM_PROP_GAIN, SYM_PROP_Q });
    }
};

void setup_flt_c_peak_tilde()
{
    SoundExternalFactory<FltCPeak> obj("flt.c_peak~", OBJECT_FACTORY_DEFAULT);
}

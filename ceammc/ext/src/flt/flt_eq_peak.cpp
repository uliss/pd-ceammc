#include "flt_eq_peak.h"
#include "ceammc_factory.h"

using namespace ceammc;

static t_symbol* SYM_PROP_FREQ = gensym("@freq");
static t_symbol* SYM_PROP_GAIN = gensym("@gain");
static t_symbol* SYM_PROP_BANDWIDTH = gensym("@bandwidth");

class FxEqPeak : public faust_flt_eq_peak_tilde {
public:
    FxEqPeak(const PdArgs& args)
        : faust_flt_eq_peak_tilde(args)
    {
        bindPositionalArgsToProps({ SYM_PROP_FREQ, SYM_PROP_GAIN, SYM_PROP_BANDWIDTH });
    }
};

void setup_flt_eq_peak_tilde()
{
    SoundExternalFactory<FxEqPeak> obj("flt.eq_peak~");
}

//EXTERNAL_NEW
//{
//    FAUST_EXT* x = reinterpret_cast<FAUST_EXT*>(pd_new(FAUST_EXT_CLASS));
//    PdArgParser p(x, argc, argv);
//    p.initFloatArg("freq", 1);
//    p.initFloatArg("gain", 2);
//    p.initFloatArg("bandwidth", 3);
//    return p.pd_obj();
//}

//EXTERNAL_SETUP(flt);

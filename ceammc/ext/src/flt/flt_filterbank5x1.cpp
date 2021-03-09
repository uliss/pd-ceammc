#include "flt_filterbank5x1.h"
#include "ceammc_factory.h"

#include <array>

using namespace ceammc;

static t_symbol* SYM_PROP_FREQ_250;
static t_symbol* SYM_PROP_FREQ_500;
static t_symbol* SYM_PROP_FREQ_1000;
static t_symbol* SYM_PROP_FREQ_2000;
static t_symbol* SYM_PROP_FREQ_4000;

static std::array<t_symbol**, 5> freq_props {
    &SYM_PROP_FREQ_250,
    &SYM_PROP_FREQ_500,
    &SYM_PROP_FREQ_1000,
    &SYM_PROP_FREQ_2000,
    &SYM_PROP_FREQ_4000,
};

class FltFilterBank5x1 : public faust_flt_filterbank5x1_tilde {
public:
    FltFilterBank5x1(const PdArgs& args)
        : faust_flt_filterbank5x1_tilde(args)
    {
        bindPositionalArgsToProps({
            SYM_PROP_FREQ_250,
            SYM_PROP_FREQ_500,
            SYM_PROP_FREQ_1000,
            SYM_PROP_FREQ_2000,
            SYM_PROP_FREQ_4000,
        });
    }

    void onList(const AtomList& lst) override
    {
        const size_t N = std::min<size_t>(lst.size(), freq_props.size());
        for (size_t i = 0; i < N; i++) {
            setProperty(*freq_props[i], lst.view(i, 1));
        }
    }
};

void setup_flt_filterbank5x1_tilde()
{
    SYM_PROP_FREQ_250 = gensym("@f250");
    SYM_PROP_FREQ_500 = gensym("@f500");
    SYM_PROP_FREQ_1000 = gensym("@f1000");
    SYM_PROP_FREQ_2000 = gensym("@f2000");
    SYM_PROP_FREQ_4000 = gensym("@f4000");

    SoundExternalFactory<FltFilterBank5x1> obj("flt.fbank5x1~");
    obj.addMethod("reset", &FltFilterBank5x1::m_reset);
    obj.setXletsInfo({ "input signal" },
        {
            "LowPass: 250Hz",
            "BandPass: 500Hz",
            "BandPass: 1000Hz",
            "BandPass: 2000Hz",
            "HighPass: 4000Hz",
        });
}

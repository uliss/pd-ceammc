#include "noise_lfreq.h"
#include "ceammc_factory.h"

using namespace ceammc;

static t_symbol* SYM_PROP_FREQ = gensym("@freq");

class NoiseLfreq : public faust_noise_lfreq_tilde {
public:
    NoiseLfreq(const PdArgs& args)
        : faust_noise_lfreq_tilde(args)
    {
        bindPositionalArgsToProps({ SYM_PROP_FREQ });
    }

    void onFloat(t_float f) override
    {
        setProperty(SYM_PROP_FREQ, AtomList(Atom(f)));
    }
};

void setup_noise_lfreq_tilde()
{
    SoundExternalFactory<NoiseLfreq> obj("noise.lfreq~", OBJECT_FACTORY_DEFAULT);
}

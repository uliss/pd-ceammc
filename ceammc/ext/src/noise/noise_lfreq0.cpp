#include "noise_lfreq0.h"
#include "ceammc_factory.h"

using namespace ceammc;

class NoiseLfreq0 : public faust_noise_lfreq0_tilde {
public:
    NoiseLfreq0(const PdArgs& args)
        : faust_noise_lfreq0_tilde(args)
    {
        bindPositionalArgsToProps({ gensym("@freq") });
    }

    void onFloat(t_float f) override
    {
        setProperty(gensym("@freq"), AtomList(Atom(f)));
    }
};

void setup_noise_lfreq0_tilde()
{
    SoundExternalFactory<NoiseLfreq0> obj("noise.lfreq0~", OBJECT_FACTORY_DEFAULT);

    obj.setDescription("sampled/held noise (piecewise constant)");
    obj.setCategory("noise");
    obj.setKeywords({"noise", "white"});
}

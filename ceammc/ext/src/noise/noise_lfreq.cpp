#include "noise_lfreq.h"
#include "ceammc_factory.h"

using namespace ceammc;

class NoiseLfreq : public faust_noise_lfreq_tilde {
public:
    NoiseLfreq(const PdArgs& args)
        : faust_noise_lfreq_tilde(args)
    {
        bindPositionalArgsToProps({ gensym("@freq") });
    }

    void onFloat(t_float f) override
    {
        setProperty(gensym("@freq"), AtomList(Atom(f)));
    }
};

void setup_noise_lfreq_tilde()
{
    SoundExternalFactory<NoiseLfreq> obj("noise.lfreq~", OBJECT_FACTORY_DEFAULT);

    obj.setDescription("noise.lfreq0~ smoothed with no overshoot");
    obj.setCategory("noise");
    obj.setKeywords({"noise", "lfreq"});
}

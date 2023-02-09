#include "noise_crackle.h"
#include "ceammc_factory.h"

using namespace ceammc;

class NoiseCrackle : public faust_noise_crackle_tilde {
public:
    NoiseCrackle(const PdArgs& args)
        : faust_noise_crackle_tilde(args)
    {
        bindPositionalArgsToProps({ gensym("@freq") });
    }

    void onFloat(t_float f) override
    {
        setProperty(gensym("@freq"), AtomList(Atom(f)));
    }
};

void setup_noise_crackle_tilde()
{
    SoundExternalFactory<NoiseCrackle> obj("noise.crackle~", OBJECT_FACTORY_DEFAULT);

    obj.setDescription("sparse noise generator");
    obj.setCategory("noise");
    obj.setKeywords({"noise", "crackle"});
}

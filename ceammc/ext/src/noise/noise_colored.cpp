#include "noise_colored.h"
#include "ceammc_factory.h"

using namespace ceammc;

class NoiseColored : public faust_noise_colored_tilde {
public:
    NoiseColored(const PdArgs& args)
        : faust_noise_colored_tilde(args)
    {
        bindPositionalArgsToProps({ gensym("@alpha") });
    }

    void onFloat(t_float f) override
    {
        setProperty(gensym("@alpha"), AtomList(Atom(f)));
    }
};

void setup_noise_colored_tilde()
{
    SoundExternalFactory<NoiseColored> obj("noise.colored~", OBJECT_FACTORY_DEFAULT);
    obj.setXletsInfo({ "float: set roll-off factor" }, { "signal: output" });

    obj.setDescription("colored noise generator with an arbitrary spectral roll");
    obj.setCategory("noise");
    obj.setKeywords({"noise", "colored", "white", "red", "brown", "blue", "velvet"});
}

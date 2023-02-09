#include "fx_wahwah.h"
#include "ceammc_factory.h"

using namespace ceammc;

class FxWahwah : public faust_fx_wahwah_tilde {
public:
    FxWahwah(const PdArgs& args)
        : faust_fx_wahwah_tilde(args)
    {
        static t_symbol* SYM_PROP_ANGLE = gensym("@angle");
        static t_symbol* SYM_PROP_SPEED = gensym("@speed");

        bindPositionalArgsToProps({ SYM_PROP_ANGLE, SYM_PROP_SPEED });
    }
};

void setup_fx_wahwah_tilde()
{
    SoundExternalFactory<FxWahwah> obj("fx.wahwah~");

    obj.setDescription("digitized CryBaby wah pedal");
    obj.setCategory("fx");
    obj.setKeywords({"wahwah", "guitar", "crybaby"});
}

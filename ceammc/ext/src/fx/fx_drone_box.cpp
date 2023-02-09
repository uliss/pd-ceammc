#include "fx_drone_box.h"
#include "ceammc_factory.h"

using namespace ceammc;

class FxDroneBox : public faust_fx_drone_box_tilde {
public:
    FxDroneBox(const PdArgs& args)
        : faust_fx_drone_box_tilde(args)
    {
        static t_symbol* SYM_PROP_PITCH = gensym("@pitch");

        bindPositionalArgsToProps({ SYM_PROP_PITCH });
    }
};

void setup_fx_drone_box_tilde()
{
    SoundExternalFactory<FxDroneBox> obj("fx.drone_box~");

    obj.setDescription("mono sympathetic resonance generator");
    obj.setCategory("fx");
    obj.setKeywords({"fx", "drone"});
}

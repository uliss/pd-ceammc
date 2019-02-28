#include "fx_drone_box.h"
#include "ceammc_factory.h"

using namespace ceammc;

static t_symbol* SYM_PROP_PITCH = gensym("@pitch");

class FxDroneBox : public faust_fx_drone_box_tilde {
public:
    FxDroneBox(const PdArgs& args)
        : faust_fx_drone_box_tilde(args)
    {
        bindPositionalArgsToProps({ SYM_PROP_PITCH });
    }
};

void setup_fx_drone_box_tilde()
{
    SoundExternalFactory<FxDroneBox> obj("fx.drone_box~");
}

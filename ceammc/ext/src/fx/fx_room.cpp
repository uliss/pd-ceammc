#include "fx_room.h"
#include "ceammc_factory.h"

using namespace ceammc;

class FxRoom : public faust_fx_room_tilde {
public:
    FxRoom(const PdArgs& args)
        : faust_fx_room_tilde(args)
    {
        bindPositionalArgsToProps({ gensym("@size"), gensym("@fb") });
    }
};

void setup_fx_room_tilde()
{
    SoundExternalFactory<FxRoom> obj("fx.room~");
    obj.addMethod("reset", &FxRoom::m_reset);
}

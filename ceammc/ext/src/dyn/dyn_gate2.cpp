#include "dyn_gate2.h"
#include "ceammc_factory.h"
using namespace ceammc;

class DynGate2 : public faust_dyn_gate2_tilde {
public:
    DynGate2(const PdArgs& args)
        : faust_dyn_gate2_tilde(args)
    {
        bindPositionalArgsToProps({ gensym("@threshold"),
            gensym("@attack"),
            gensym("@hold"),
            gensym("@release") });
    }
};

void setup_dyn_gate2_tilde()
{
    SoundExternalFactory<DynGate2> obj("dyn.gate2~");

    obj.setDescription("stereo signal gate");
    obj.setCategory("dyn");
    obj.setKeywords({"gate"});
}

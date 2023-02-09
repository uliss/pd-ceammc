#include "dyn_gate.h"
#include "ceammc_factory.h"
using namespace ceammc;

class DynGate : public faust_dyn_gate_tilde {
public:
    DynGate(const PdArgs& args)
        : faust_dyn_gate_tilde(args)
    {
        bindPositionalArgsToProps({ gensym("@threshold"),
            gensym("@attack"),
            gensym("@hold"),
            gensym("@release") });
    }
};

void setup_dyn_gate_tilde()
{
    SoundExternalFactory<DynGate> obj("dyn.gate~");

    obj.setDescription("mono signal gate");
    obj.setCategory("dyn");
    obj.setKeywords({"gate"});
}

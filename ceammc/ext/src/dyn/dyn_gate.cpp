#include "dyn_gate.h"
#include "ceammc_factory.h"
using namespace ceammc;

static t_symbol* SYM_PROP_THRESHOLD = gensym("@threshold");
static t_symbol* SYM_PROP_ATTACK = gensym("@attack");
static t_symbol* SYM_PROP_HOLD = gensym("@hold");
static t_symbol* SYM_PROP_RELEASE = gensym("@release");

class DynGate : public faust_dyn_gate_tilde {
public:
    DynGate(const PdArgs& args)
        : faust_dyn_gate_tilde(args)
    {
        bindPositionalArgsToProps({ SYM_PROP_THRESHOLD, SYM_PROP_ATTACK, SYM_PROP_HOLD, SYM_PROP_RELEASE });
    }
};

void setup_dyn_gate_tilde()
{
    SoundExternalFactory<DynGate> obj("dyn.gate~");
}

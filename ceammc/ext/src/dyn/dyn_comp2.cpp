#include "dyn_comp2.h"
#include "ceammc_factory.h"
using namespace ceammc;

static t_symbol* SYM_PROP_RATIO = gensym("@ratio");
static t_symbol* SYM_PROP_THRESHOLD = gensym("@threshold");
static t_symbol* SYM_PROP_ATTACK = gensym("@attack");
static t_symbol* SYM_PROP_RELEASE = gensym("@release");

class DynComp2 : public faust_dyn_comp2_tilde {
public:
    DynComp2(const PdArgs& args)
        : faust_dyn_comp2_tilde(args)
    {
        bindPositionalArgsToProps({ SYM_PROP_RATIO, SYM_PROP_THRESHOLD, SYM_PROP_ATTACK, SYM_PROP_RELEASE });
    }
};

void setup_dyn_comp2_tilde()
{
    SoundExternalFactory<DynComp2> obj("dyn.comp2~");
}

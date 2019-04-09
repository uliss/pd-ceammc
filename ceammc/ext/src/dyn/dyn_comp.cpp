#include "dyn_comp.h"
#include "ceammc_factory.h"
using namespace ceammc;

static t_symbol* SYM_PROP_RATIO = gensym("@ratio");
static t_symbol* SYM_PROP_THRESHOLD = gensym("@threshold");
static t_symbol* SYM_PROP_ATTACK = gensym("@attack");
static t_symbol* SYM_PROP_RELEASE = gensym("@release");

class DynComp : public faust_dyn_comp_tilde {
public:
    DynComp(const PdArgs& args)
        : faust_dyn_comp_tilde(args)
    {
        bindPositionalArgsToProps({ gensym("@ratio"),
            gensym("@threshold"),
            gensym("@attack"),
            gensym("@release") });
    }
};

void setup_dyn_comp_tilde()
{
    SoundExternalFactory<DynComp> obj("dyn.comp~");
}

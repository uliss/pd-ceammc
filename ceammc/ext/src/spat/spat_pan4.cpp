#include "spat_pan4.h"
#include "ceammc_factory.h"

using namespace ceammc;

static t_symbol* SYM_PROP_ANGLE = gensym("@angle");
static t_symbol* SYM_PROP_DIST = gensym("@dist");

class SpatPan4 : public faust_spat_pan4_tilde {
public:
    SpatPan4(const PdArgs& args)
        : faust_spat_pan4_tilde(args)
    {
        bindPositionalArgsToProps({ SYM_PROP_ANGLE, SYM_PROP_DIST });
    }
};

void setup_spat_pan4_tilde()
{
    SoundExternalFactory<SpatPan4> obj("spat.pan4~");
    obj.addAlias("pan4~");
}

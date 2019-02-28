#include "spat_pan8.h"
#include "ceammc_factory.h"

using namespace ceammc;

static t_symbol* SYM_PROP_ANGLE = gensym("@angle");
static t_symbol* SYM_PROP_DIST = gensym("@dist");

class SpatPan8 : public faust_spat_pan8_tilde {
public:
    SpatPan8(const PdArgs& args)
        : faust_spat_pan8_tilde(args)
    {
        bindPositionalArgsToProps({ SYM_PROP_ANGLE, SYM_PROP_DIST });
    }
};

void setup_spat_pan8_tilde()
{
    SoundExternalFactory<SpatPan8> obj("spat.pan8~");
    obj.addAlias("pan8~");
}

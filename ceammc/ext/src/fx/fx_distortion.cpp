#include "fx_distortion.h"
#include "ceammc_factory.h"

using namespace ceammc;

static t_symbol* SYM_PROP_DRIVE = gensym("drive");
static t_symbol* SYM_PROP_OFFSET = gensym("offset");

class FxDistortion : public faust_fx_distortion_tilde {
public:
    FxDistortion(const PdArgs& args)
        : faust_fx_distortion_tilde(args)
    {
        bindPositionalArgsToProps({ SYM_PROP_DRIVE, SYM_PROP_OFFSET });
    }
};

void setup_fx_distortion_tilde()
{
    SoundExternalFactory<FxDistortion> obj("fx.distortion~");
}

#include "fx_bitdown.h"
#include "ceammc_factory.h"

using namespace ceammc;

static t_symbol* SYM_PROP_BITS = gensym("@bits");
static t_symbol* SYM_PROP_DOWNSAMP = gensym("@downsamp");

class FxBitdown : public faust_fx_bitdown_tilde {
public:
    FxBitdown(const PdArgs& args)
        : faust_fx_bitdown_tilde(args)
    {
        bindPositionalArgsToProps({ SYM_PROP_BITS, SYM_PROP_DOWNSAMP });
    }
};

void setup_fx_bitdown_tilde()
{
    SoundExternalFactory<FxBitdown> obj("fx.bitdown~");
}

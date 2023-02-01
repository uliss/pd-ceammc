#include "fx_bitdown.h"
#include "ceammc_factory.h"

using namespace ceammc;

class FxBitdown : public faust_fx_bitdown_tilde {
public:
    FxBitdown(const PdArgs& args)
        : faust_fx_bitdown_tilde(args)
    {
        auto SYM_PROP_BITS = gensym("@bits");
        auto SYM_PROP_DOWNSAMP = gensym("@downsamp");

        bindPositionalArgsToProps({ SYM_PROP_BITS, SYM_PROP_DOWNSAMP });
        property(SYM_PROP_DOWNSAMP)->setUnitsSamp();
    }
};

void setup_fx_bitdown_tilde()
{
    SoundExternalFactory<FxBitdown> obj("fx.bitdown~");
}

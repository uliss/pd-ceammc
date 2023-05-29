#include "fx_shimmer.h"
#include "ceammc_faust_factory.h"

using namespace ceammc;

class FxShimmer : public faust_fx_shimmer_tilde {
public:
    FxShimmer(const PdArgs& args)
        : faust_fx_shimmer_tilde(args)
    {
        bindPositionalArgsToProps({ gensym("@decay_low"), gensym("@decay_mid") });
    }
};

void setup_fx_shimmer_tilde()
{
    FaustFactory<FxShimmer> obj("fx.shimmer~");

    obj.setDescription("shimmer reverb effect effect");
    obj.setCategory("fx");
    obj.setKeywords({ "fx", "echo", "shimmer", "reverb" });
}

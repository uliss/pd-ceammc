#include "fx_scrambler.h"
#include "ceammc_factory.h"
#include "ceammc_faust_factory.h"

using namespace ceammc;

class FxScrambler : public faust_fx_scrambler_tilde {
public:
    FxScrambler(const PdArgs& args)
        : faust_fx_scrambler_tilde(args)
    {
        bindPositionalArgsToProps({ "@freq1", "@freq2" });
    }
};

void setup_fx_scrambler_tilde()
{
    FaustFactory<FxScrambler> obj("fx.scramble~");

    //    obj.setDescription("Chorus effect");
    obj.setCategory("fx");
    //    obj.setKeywords({ "fx", "chorus" });
}

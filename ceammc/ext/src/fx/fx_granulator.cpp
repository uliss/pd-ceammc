#include "fx_granulator.h"
#include "ceammc_faust_factory.h"

using namespace ceammc;

class FxGranulator : public faust_fx_granulator_tilde {
public:
    FxGranulator(const PdArgs& args)
        : faust_fx_granulator_tilde(args)
    {
    }
};

void setup_fx_granulator_tilde()
{
    FaustFactory<FxGranulator> obj("fx.granulator~");

    obj.setDescription("simple input stream granulator");
    obj.setCategory("fx");
    obj.setKeywords({ "fx", "bits", "granulator" });
}

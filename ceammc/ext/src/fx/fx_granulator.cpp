#include "fx_granulator.h"
#include "ceammc_factory.h"

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
    SoundExternalFactory<FxGranulator> obj("fx.granulator~");
    obj.addMethod("reset", &FxGranulator::m_reset);
}

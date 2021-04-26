#include "fx_recho.h"
#include "ceammc_factory.h"

using namespace ceammc;

class FxREcho : public faust_fx_recho_tilde {
public:
    FxREcho(const PdArgs& args)
        : faust_fx_recho_tilde(args)
    {
    }
};

void setup_fx_recho_tilde()
{
    SoundExternalFactory<FxREcho> obj("fx.recho~");
    obj.addMethod("reset", &FxREcho::m_reset);
}

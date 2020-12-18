#include "spat_zita6x8.h"
#include "ceammc_factory.h"

using namespace ceammc;

class SpatZita6x8 : public faust_spat_zita6x8_tilde {
public:
    SpatZita6x8(const PdArgs& args)
        : faust_spat_zita6x8_tilde(args)
    {
    }
};

void setup_spat_zita6x8_tilde()
{
    SoundExternalFactory<SpatZita6x8> obj("spat.zita6x8~");
    obj.addMethod("reset", &SpatZita6x8::m_reset);
}

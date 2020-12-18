#include "spat_zita8.h"
#include "ceammc_factory.h"

using namespace ceammc;

class SpatZita8 : public faust_spat_zita8_tilde {
public:
    SpatZita8(const PdArgs& args)
        : faust_spat_zita8_tilde(args)
    {
        bindPositionalArgsToProps({ gensym("@radius"), gensym("@angle") });
    }
};

void setup_spat_zita8_tilde()
{
    SoundExternalFactory<SpatZita8> obj("spat.zita8~");
    obj.addMethod("reset", &SpatZita8::m_reset);
}

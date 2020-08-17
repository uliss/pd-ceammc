#include "spat_pan8.h"
#include "ceammc_factory.h"

using namespace ceammc;

class SpatPan8 : public faust_spat_pan8_tilde {
public:
    SpatPan8(const PdArgs& args)
        : faust_spat_pan8_tilde(args)
    {
        bindPositionalArgsToProps({ gensym("@radius"), gensym("@angle") });
    }
};

void setup_spat_pan8_tilde()
{
    SoundExternalFactory<SpatPan8> obj("spat.pan8~");
    obj.addAlias("pan8~");
}

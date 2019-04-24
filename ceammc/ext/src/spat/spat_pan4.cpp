#include "spat_pan4.h"
#include "ceammc_factory.h"

using namespace ceammc;

class SpatPan4 : public faust_spat_pan4_tilde {
public:
    SpatPan4(const PdArgs& args)
        : faust_spat_pan4_tilde(args)
    {
        bindPositionalArgsToProps({ gensym("@angle"), gensym("@dist") });
    }
};

void setup_spat_pan4_tilde()
{
    SoundExternalFactory<SpatPan4> obj("spat.pan4~");
    obj.addAlias("pan4~");
}

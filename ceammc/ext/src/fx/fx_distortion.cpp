#include "fx_distortion.h"
#include "ceammc_factory.h"

using namespace ceammc;

class FxDistortion : public faust_fx_distortion_tilde {
public:
    FxDistortion(const PdArgs& args)
        : faust_fx_distortion_tilde(args)
    {
        static t_symbol* SYM_PROP_DRIVE = gensym("@drive");
        static t_symbol* SYM_PROP_OFFSET = gensym("@offset");

        bindPositionalArgsToProps({ SYM_PROP_DRIVE, SYM_PROP_OFFSET });
    }
};

void setup_fx_distortion_tilde()
{
    SoundExternalFactory<FxDistortion> obj("fx.distortion~");

    obj.setDescription("cubic nonlinearity distortion");
    obj.setCategory("fx");
    obj.setKeywords({"fx", "distortion"});
}

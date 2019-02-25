#include "fx_vocoder.h"
#include "ceammc_factory.h"

using namespace ceammc;

//static t_symbol* SYM_PROP_DRIVE = gensym("@drive");

class FxVocoder : public faust_fx_vocoder_tilde {
public:
    FxVocoder(const PdArgs& args)
        : faust_fx_vocoder_tilde(args)
    {
        //        bindPositionalArgsToProps({ SYM_PROP_DRIVE });
    }
};

void setup_fx_vocoder_tilde()
{
    SoundExternalFactory<FxVocoder> obj("fx.vocoder~");
}

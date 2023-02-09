#include "dyn_comp2.h"
#include "ceammc_factory.h"
using namespace ceammc;

class DynComp2 : public faust_dyn_comp2_tilde {
public:
    DynComp2(const PdArgs& args)
        : faust_dyn_comp2_tilde(args)
    {
        bindPositionalArgsToProps({ gensym("@ratio"),
            gensym("@threshold"),
            gensym("@attack"),
            gensym("@release") });
    }
};

void setup_dyn_comp2_tilde()
{
    SoundExternalFactory<DynComp2> obj("dyn.comp2~");

    obj.setDescription("stereo dynamic range compressor");
    obj.setCategory("dyn");
    obj.setKeywords({"compressor"});
}

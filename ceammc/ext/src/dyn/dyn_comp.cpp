#include "dyn_comp.h"
#include "ceammc_factory.h"
using namespace ceammc;

class DynComp : public faust_dyn_comp_tilde {
public:
    DynComp(const PdArgs& args)
        : faust_dyn_comp_tilde(args)
    {
        bindPositionalArgsToProps({ gensym("@ratio"),
            gensym("@threshold"),
            gensym("@attack"),
            gensym("@release") });
    }
};

void setup_dyn_comp_tilde()
{
    SoundExternalFactory<DynComp> obj("dyn.comp~");

    obj.setDescription("mono dynamic range compressors");
    obj.setCategory("dyn");
    obj.setKeywords({"compressor"});
}

#include "flt_c_lowshelf.h"
#include "ceammc_factory.h"

using namespace ceammc;

class FltCLowShelf : public faust_flt_c_lowshelf_tilde {
public:
    FltCLowShelf(const PdArgs& args)
        : faust_flt_c_lowshelf_tilde(args)
    {
        bindPositionalArgsToProps({ gensym("@freq"), gensym("@gain") });
        property("@q")->setVisibility(PropValueVis::INTERNAL);
    }
};

void setup_flt_c_lowshelf_tilde()
{
    SoundExternalFactory<FltCLowShelf> obj("flt.c_lowshelf~", OBJECT_FACTORY_DEFAULT);
}

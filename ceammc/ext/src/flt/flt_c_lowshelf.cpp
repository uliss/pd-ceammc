#include "flt_c_lowshelf.h"
#include "ceammc_factory.h"

using namespace ceammc;

static t_symbol* SYM_PROP_FREQ = gensym("@freq");
static t_symbol* SYM_PROP_GAIN = gensym("@gain");

class FltCLowShelf : public faust_flt_c_lowshelf_tilde {
public:
    FltCLowShelf(const PdArgs& args)
        : faust_flt_c_lowshelf_tilde(args)
    {
        bindPositionalArgsToProps({ SYM_PROP_FREQ, SYM_PROP_GAIN });
    }
};

void setup_flt_c_lowshelf_tilde()
{
    SoundExternalFactory<FltCLowShelf> obj("flt.c_lowshelf~", OBJECT_FACTORY_DEFAULT);
}

#include "flt_c_highshelf.h"
#include "ceammc_factory.h"

using namespace ceammc;

static t_symbol* SYM_PROP_FREQ = gensym("@freq");
static t_symbol* SYM_PROP_GAIN = gensym("@gain");

class FltCHighShelf : public faust_flt_c_highshelf_tilde {
public:
    FltCHighShelf(const PdArgs& args)
        : faust_flt_c_highshelf_tilde(args)
    {
        bindPositionalArgsToProps({ SYM_PROP_FREQ, SYM_PROP_GAIN });
    }
};

void setup_flt_c_highshelf_tilde()
{
    SoundExternalFactory<FltCHighShelf> obj("flt.c_highshelf~", OBJECT_FACTORY_DEFAULT);
}

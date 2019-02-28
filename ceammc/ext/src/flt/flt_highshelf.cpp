#include "flt_highshelf.h"
#include "ceammc_factory.h"
using namespace ceammc;

static t_symbol* SYM_PROP_FREQ = gensym("@freq");
static t_symbol* SYM_PROP_GAIN = gensym("@gain");

class FltHighShelf : public faust_flt_highshelf_tilde {
public:
    FltHighShelf(const PdArgs& args)
        : faust_flt_highshelf_tilde(args)
    {
        bindPositionalArgsToProps({ SYM_PROP_FREQ, SYM_PROP_GAIN });
    }
};

void setup_flt_highshelf_tilde()
{
    SoundExternalFactory<FltHighShelf> obj("flt.highshelf~");
}

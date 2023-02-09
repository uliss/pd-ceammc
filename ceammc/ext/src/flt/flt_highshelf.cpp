#include "flt_highshelf.h"
#include "ceammc_factory.h"
using namespace ceammc;

class FltHighShelf : public faust_flt_highshelf_tilde {
public:
    FltHighShelf(const PdArgs& args)
        : faust_flt_highshelf_tilde(args)
    {
        static t_symbol* SYM_PROP_FREQ = gensym("@freq");
        static t_symbol* SYM_PROP_GAIN = gensym("@gain");

        bindPositionalArgsToProps({ SYM_PROP_FREQ, SYM_PROP_GAIN });
    }
};

void setup_flt_highshelf_tilde()
{
    SoundExternalFactory<FltHighShelf> obj("flt.highshelf~");

    obj.setDescription("gain boost|cut above some frequency");
    obj.setCategory("flt");
    obj.setKeywords({"filter", "highshelf"});
}

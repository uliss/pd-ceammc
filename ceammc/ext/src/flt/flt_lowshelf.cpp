#include "flt_lowshelf.h"
#include "ceammc_factory.h"

using namespace ceammc;

class FltLowShelf : public faust_flt_lowshelf_tilde {
public:
    FltLowShelf(const PdArgs& args)
        : faust_flt_lowshelf_tilde(args)
    {
        static t_symbol* SYM_PROP_FREQ = gensym("@freq");
        static t_symbol* SYM_PROP_GAIN = gensym("@gain");

        bindPositionalArgsToProps({ SYM_PROP_FREQ, SYM_PROP_GAIN });
    }
};

void setup_flt_lowshelf_tilde()
{
    SoundExternalFactory<FltLowShelf> obj("flt.lowshelf~");

    obj.setDescription("gain boost|cut below some frequency");
    obj.setCategory("flt");
    obj.setKeywords({"filter", "lowshelf"});
}

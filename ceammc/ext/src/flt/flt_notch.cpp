#include "flt_notch.h"
#include "ceammc_factory.h"

using namespace ceammc;

class FltNotch : public faust_flt_notch_tilde {
public:
    FltNotch(const PdArgs& args)
        : faust_flt_notch_tilde(args)
    {
        static t_symbol* SYM_PROP_FREQ = gensym("@freq");
        static t_symbol* SYM_PROP_WIDTH = gensym("@width");

        bindPositionalArgsToProps({ SYM_PROP_FREQ, SYM_PROP_WIDTH });
    }
};

void setup_flt_notch_tilde()
{
    SoundExternalFactory<FltNotch> obj("flt.notch~");

    obj.setDescription("band-rejection filter");
    obj.setCategory("flt");
    obj.setKeywords({"filter", "lowshelf"});
}

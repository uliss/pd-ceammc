#include "flt_c_notch.h"
#include "ceammc_factory.h"

using namespace ceammc;

class FltCNotch : public faust_flt_c_notch_tilde {
public:
    FltCNotch(const PdArgs& args)
        : faust_flt_c_notch_tilde(args)
    {
        bindPositionalArgsToProps({ gensym("@freq"), gensym("@q") });
    }
};

void setup_flt_c_notch_tilde()
{
    SoundExternalFactory<FltCNotch> obj("flt.c_notch~", OBJECT_FACTORY_DEFAULT);
}

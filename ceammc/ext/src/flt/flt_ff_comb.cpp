#include "flt_ff_comb.h"
#include "ceammc_factory.h"

using namespace ceammc;

class FltFfComb : public faust_flt_ff_comb_tilde {
public:
    FltFfComb(const PdArgs& args)
        : faust_flt_ff_comb_tilde(args)
    {
        auto p = property("@a");
        if (p)
            p->setArgIndex(0);
    }
};

void setup_flt_ff_comb_tilde()
{
    SoundExternalFactory<FltFfComb> obj("flt.ff_comb~");
    obj.addMethod("reset", &FltFfComb::m_reset);
}

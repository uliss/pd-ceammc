#include "flt_biquad.h"
#include "ceammc_factory.h"

using namespace ceammc;

class FltBiquad : public faust_flt_biquad_tilde {
public:
    FltBiquad(const PdArgs& args)
        : faust_flt_biquad_tilde(args)
    {
    }
};

void setup_flt_biquad_tilde()
{
    SoundExternalFactory<FltBiquad> obj("flt.biquad~");
    obj.addMethod("reset", &FltBiquad::m_reset);
}

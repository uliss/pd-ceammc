#include "fx_pitchshift_sig.h"
#include "ceammc_faust_factory.h"

using namespace ceammc;

class FxPitchShiftSig : public faust_fx_pitchshift_sig_tilde {
public:
    FxPitchShiftSig(const PdArgs& args)
        : faust_fx_pitchshift_sig_tilde(args)
    {
    }
};

void setup_fx_pitchshift_sig_tilde()
{
    FaustFactory<FxPitchShiftSig> obj("fx.pitchshift_s~");

    obj.setDescription("signal version of fx.pitchshift~");
    obj.setCategory("fx");
    obj.setKeywords({ "fx", "pitchshift" });
}

#include "fx_freqshift.h"
#include "ceammc_factory.h"

using namespace ceammc;

class FxFreqShift : public faust_fx_freqshift_tilde {
public:
    FxFreqShift(const PdArgs& args)
        : faust_fx_freqshift_tilde(args)
    {
    }
};

void setup_fx_freqshift_tilde()
{
    SoundExternalFactory<FxFreqShift> obj("fx.freqshift~");

    obj.setXletsInfo({ "input signal", "freqshift (hz)" }, { "output (freq+shift)", "output (freq-shift)" });

    obj.setDescription("frequency shifter or single-sideband ring modulation");
    obj.setCategory("fx");
    obj.setKeywords({"fx", "freqshift"});
}

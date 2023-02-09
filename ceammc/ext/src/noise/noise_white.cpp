#include "noise_white.h"

#include "ceammc_factory.h"

using namespace ceammc;

class WhiteNoise : public faust_noise_white_tilde {
public:
    WhiteNoise(const PdArgs& args)
        : faust_noise_white_tilde(args)
    {
    }
};

void setup_noise_white_tilde()
{
    SoundExternalFactory<WhiteNoise> obj("noise.white~", OBJECT_FACTORY_NO_DEFAULT_INLET);

    obj.setDescription("White noise generator");
    obj.setCategory("noise");
    obj.setKeywords({"noise", "white"});
}

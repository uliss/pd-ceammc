#include "noise_pink.h"
#include "ceammc_factory.h"

using namespace ceammc;

class PinkNoise : public faust_noise_pink_tilde {
public:
    PinkNoise(const PdArgs& args)
        : faust_noise_pink_tilde(args)
    {
    }
};

void setup_noise_pink_tilde()
{
    SoundExternalFactory<PinkNoise> obj("noise.pink~", OBJECT_FACTORY_NO_DEFAULT_INLET);
}

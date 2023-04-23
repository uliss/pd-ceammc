#include "noise_crackle.h"
#include "ceammc_factory.h"

using namespace ceammc;

constexpr const char* PROP_FREQ = "@freq";

class NoiseCrackle : public faust_noise_crackle_tilde {
    UIProperty* freq_ { nullptr };

public:
    NoiseCrackle(const PdArgs& args)
        : faust_noise_crackle_tilde(args)
        , freq_(findUIProperty("@freq"))
    {
        bindPositionalArgsToProps({ PROP_FREQ });
    }

    void onFloat(t_float f) override
    {
        if (freq_)
            freq_->setValue(f, true);
    }
};

void setup_noise_crackle_tilde()
{
    SoundExternalFactory<NoiseCrackle> obj("noise.crackle~", OBJECT_FACTORY_DEFAULT);
    obj.setXletsInfo({ "float: freq of impulses in Hz" }, { "signal: random impulses in -1..+1 range" });

    obj.setDescription("sparse noise generator");
    obj.setCategory("noise");
    obj.setKeywords({ "noise", "crackle" });
}

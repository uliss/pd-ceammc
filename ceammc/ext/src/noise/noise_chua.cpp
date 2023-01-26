#include "noise_chua.h"
#include "ceammc_clock.h"
#include "ceammc_factory.h"

#include <cmath>

using namespace ceammc;

class NoiseChua : public faust_noise_chua_tilde {
    UIProperty* gate_;
    ClockLambdaFunction clock_;

public:
    NoiseChua(const PdArgs& args)
        : faust_noise_chua_tilde(args)
        , gate_(static_cast<UIProperty*>(property("@gate")))
        , clock_([this]() {
            if (gate_)
                gate_->setValue(1);
        })
    {
    }

    void processBlock(const t_sample** in, t_sample** out) final
    {
        faust_noise_chua_tilde::processBlock(in, out);

        if (std::isnan(out[0][0]))
            initConstants();
    }

    void m_start(t_symbol*, const AtomListView&)
    {
        clock_.delay(0);
    }

    void m_reset(t_symbol*, const AtomListView& lv)
    {
        dsp_->init(samplerate());
    }
};

void setup_noise_chua_tilde()
{
    SoundExternalFactory<NoiseChua> obj("noise.chua~", OBJECT_FACTORY_DEFAULT);
    obj.addMethod("reset", &NoiseChua::m_reset);
    obj.addMethod("start", &NoiseChua::m_start);
}

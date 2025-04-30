#include "lfo_impulse.h"
#include "ceammc_factory.h"

using namespace ceammc;

class LfoImpulse : public faust_lfo_impulse_tilde {
    FloatProperty* freq_ { nullptr };

public:
    explicit LfoImpulse(const PdArgs& args)
        : faust_lfo_impulse_tilde(args)
    {
        createInlet();

        freq_ = new FloatProperty("@freq", 0);
        freq_->setSuccessFn([this](Property*) {
            setInitSignalValue(freq_->value());
        });
        freq_->setUnitsHz();
        freq_->checkClosedRange(0, 1000);
        freq_->setArgIndex(0);
        addProperty(freq_);
    }

    void onInlet(size_t n, const AtomListView&) override
    {
        dsp_->instanceClear();
    }
};

void setup_lfo_impulse_tilde()
{
    SoundExternalFactory<LfoImpulse> obj("lfo.impulse~");
}

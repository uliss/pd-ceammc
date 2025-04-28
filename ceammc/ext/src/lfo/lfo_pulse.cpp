#include "lfo_pulse.h"
#include "ceammc_factory.h"
using namespace ceammc;

class LfoPulse : public faust_lfo_pulse_tilde {
    FloatProperty* freq_ { nullptr };

public:
    explicit LfoPulse(const PdArgs& args)
        : faust_lfo_pulse_tilde(args)
        , freq_(nullptr)
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

        bindPositionalArgToProperty(1, gensym("@duty"));
    }

    void onInlet(size_t n, const AtomListView&) override
    {
        dsp_->instanceClear();
    }
};

void setup_lfo_pulse_tilde()
{
    SoundExternalFactory<LfoPulse> obj("lfo.pulse~");
    obj.addMethod("reset", &LfoPulse::m_reset);
    obj.setXletsInfo({ "freq in hz", "bang: reset phase" }, { "pulse wave in \\[-1,+1\\] range" });
}

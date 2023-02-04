#include "lfo_pulse.h"
#include "ceammc_factory.h"
using namespace ceammc;

class LfoPulse : public faust_lfo_pulse_tilde {
    FloatProperty* freq_;

public:
    LfoPulse(const PdArgs& args)
        : faust_lfo_pulse_tilde(args)
        , freq_(nullptr)
    {
        createInlet();

        freq_ = new FloatProperty("@initfreq", 0);
        freq_->setInitOnly();
        freq_->setUnitsHz();
        freq_->setSuccessFn([this](Property*) { setInitSignalValue(freq_->value()); });
        addProperty(freq_);

        bindPositionalArgsToProps({ gensym("@initfreq"), gensym("@duty") });
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

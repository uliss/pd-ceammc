#include "lfo_pulse_pos.h"
#include "ceammc_factory.h"
using namespace ceammc;

class LfoPulsePos : public faust_lfo_pulse_pos_tilde {
    FloatProperty* freq_ { nullptr };

public:
    explicit LfoPulsePos(const PdArgs& args)
        : faust_lfo_pulse_pos_tilde(args)
    {
        createInlet();

        freq_ = new FloatProperty("@freq", 0);
        freq_->setSuccessFn([this](Property*) {
            setInitSignalValue(freq_->value());
        });
        freq_->setUnitsHz();
        freq_->setArgIndex(0);
        freq_->checkClosedRange(0, 1000);
        addProperty(freq_);

        bindPositionalArgToProperty(1, gensym("@duty"));
    }

    void onInlet(size_t n, const AtomListView&) override
    {
        dsp_->instanceClear();
    }
};

void setup_lfo_pulse_pos_tilde()
{
    SoundExternalFactory<LfoPulsePos> obj("lfo.+pulse~");
    obj.addMethod("reset", &LfoPulsePos::m_reset);
    obj.setXletsInfo({ "freq in hz", "bang: reset phase" }, { "pulse wave in \\[0,+1\\] range" });
}

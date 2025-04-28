#include "lfo_pulse_pos.h"
#include "ceammc_factory.h"
using namespace ceammc;

class LfoPulsePos : public faust_lfo_pulse_pos_tilde {
public:
    explicit LfoPulsePos(const PdArgs& args)
        : faust_lfo_pulse_pos_tilde(args)
    {
        createInlet();

        auto freq = new FloatProperty("@freq", 0);
        freq->setSuccessFn([this, freq](Property*) {
            setInitSignalValue(freq->value());
        });
        freq->setUnitsHz();
        freq->setArgIndex(0);
        freq->checkClosedRange(0, 1000);
        addProperty(freq);

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

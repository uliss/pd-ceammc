#include "lfo_saw_pos.h"
#include "ceammc_factory.h"
using namespace ceammc;

class LfoSawPos : public faust_lfo_saw_pos_tilde {
    FloatProperty* freq_ { nullptr };

public:
    explicit LfoSawPos(const PdArgs& args)
        : faust_lfo_saw_pos_tilde(args)
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

        bindPositionalArgToProperty(1, gensym("@invert"));
    }

    void onInlet(size_t n, const AtomListView&) override
    {
        dsp_->instanceClear();
    }
};

void setup_lfo_saw_pos_tilde()
{
    SoundExternalFactory<LfoSawPos> obj("lfo.+saw~");
    obj.addMethod("reset", &LfoSawPos::m_reset);
    obj.setXletsInfo({ "freq in hz", "bang: reset phase" }, { "saw in \\[0,+1) range" });
}

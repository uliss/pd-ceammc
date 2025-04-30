#include "lfo_tri_pos.h"
#include "ceammc_factory.h"

using namespace ceammc;

class LfoTriPos : public faust_lfo_tri_pos_tilde {
    FloatProperty* freq_ { nullptr };

public:
    explicit LfoTriPos(const PdArgs& args)
        : faust_lfo_tri_pos_tilde(args)
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

void setup_lfo_tri_pos_tilde()
{
    SoundExternalFactory<LfoTriPos> obj("lfo.+tri~");
    obj.addMethod("reset", &LfoTriPos::m_reset);
    obj.setXletsInfo({ "freq in hz", "bang: reset phase" }, { "triangle wave in \\[0,+1) range" });
}

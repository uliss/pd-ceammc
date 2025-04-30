#include "lfo_tri.h"
#include "ceammc_factory.h"

using namespace ceammc;

class LfoTri : public faust_lfo_tri_tilde {
    FloatProperty* freq_ { nullptr };

public:
    explicit LfoTri(const PdArgs& args)
        : faust_lfo_tri_tilde(args)
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

void setup_lfo_tri_tilde()
{
    SoundExternalFactory<LfoTri> obj("lfo.tri~");
    obj.addMethod("reset", &LfoTri::m_reset);
    obj.setXletsInfo({ "freq in hz", "bang: reset phase" }, { "triangle wave in \\[-1,+1) range" });
}

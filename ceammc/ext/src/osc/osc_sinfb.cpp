#include "osc_sinfb.h"
#include "ceammc_factory.h"

using namespace ceammc;

class OscSinFb : public faust_osc_sinfb_tilde {
    FloatProperty* freq_ { nullptr };

public:
    explicit OscSinFb(const PdArgs& args)
        : faust_osc_sinfb_tilde(args)
    {
        createInlet();

        freq_ = new FloatProperty("@freq", 0);
        freq_->setSuccessFn([this](Property*) {
            setInitSignalValue(freq_->value());
        });
        freq_->setUnitsHz();
        freq_->checkClosedRange(0, samplerate() / 2);
        freq_->setArgIndex(0);
        addProperty(freq_);

        bindPositionalArgToProperty(1, gensym("@feedback"));
    }

    void onInlet(size_t n, const AtomListView&) override
    {
        dsp_->instanceClear();
    }
};

void setup_osc_sinfb_tilde()
{
    SoundExternalFactory<OscSinFb> obj("osc.sinfb~");
}

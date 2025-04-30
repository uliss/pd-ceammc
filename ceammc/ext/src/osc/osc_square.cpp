#include "osc_square.h"
#include "ceammc_factory.h"

using namespace ceammc;

class OscSquare : public faust_osc_square_tilde {
    FloatProperty* freq_ { nullptr };

public:
    explicit OscSquare(const PdArgs& args)
        : faust_osc_square_tilde(args)
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
    }

    void onInlet(size_t n, const AtomListView&) override
    {
        dsp_->instanceClear();
    }
};

void setup_osc_square_tilde()
{
    SoundExternalFactory<OscSquare> obj("osc.square~");
}

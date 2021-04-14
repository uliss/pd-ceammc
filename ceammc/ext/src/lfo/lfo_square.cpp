#include "lfo_square.h"
#include "ceammc_factory.h"
using namespace ceammc;

class LfoSquare : public faust_lfo_square_tilde {
public:
    LfoSquare(const PdArgs& args)
        : faust_lfo_square_tilde(args)
    {
        createInlet();
        setInitSignalValue(parsedPosArgs().floatAt(0, 0));
    }

    void onInlet(size_t n, const AtomListView&) override
    {
        dsp_->instanceClear();
    }
};

void setup_lfo_square_tilde()
{
    SoundExternalFactory<LfoSquare> obj("lfo.square~");
    obj.addMethod("reset", &LfoSquare::m_reset);
    obj.setXletsInfo({ "freq in hz", "bang: reset phase" }, { "square wave in \\[-1,+1\\] range" });
}

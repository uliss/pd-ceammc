#include "lfo_square_pos.h"
#include "ceammc_factory.h"
using namespace ceammc;

class LfoSquarePos : public faust_lfo_square_pos_tilde {
public:
    LfoSquarePos(const PdArgs& args)
        : faust_lfo_square_pos_tilde(args)
    {
        createInlet();
        setInitSignalValue(parsedPosArgs().floatAt(0, 0));
    }

    void onInlet(size_t n, const AtomListView&) override
    {
        dsp_->instanceClear();
    }
};

void setup_lfo_square_pos_tilde()
{
    SoundExternalFactory<LfoSquarePos> obj("lfo.+square~");
}

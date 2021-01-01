#include "lfo_tri.h"
#include "ceammc_factory.h"

using namespace ceammc;

class LfoTri : public faust_lfo_tri_tilde {
public:
    LfoTri(const PdArgs& args)
        : faust_lfo_tri_tilde(args)
    {
        createInlet();
        setInitSignalValue(parsedPosArgs().floatAt(0, 0));
    }

    void onInlet(size_t n, const AtomListView&) override
    {
        dsp_->instanceClear();
    }
};

void setup_lfo_tri_tilde()
{
    SoundExternalFactory<LfoTri> obj("lfo.tri~");
}

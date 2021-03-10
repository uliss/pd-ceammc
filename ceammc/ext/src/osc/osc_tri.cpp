#include "osc_tri.h"
#include "ceammc_factory.h"

using namespace ceammc;

class OscTri : public faust_osc_tri_tilde {
public:
    OscTri(const PdArgs& args)
        : faust_osc_tri_tilde(args)
    {
        createInlet();
        setInitSignalValue(parsedPosArgs().floatAt(0, 0));
    }

    void onInlet(size_t n, const AtomListView&) override
    {
        dsp_->instanceClear();
    }
};

void setup_osc_tri_tilde()
{
    SoundExternalFactory<OscTri> obj("osc.tri~");
}

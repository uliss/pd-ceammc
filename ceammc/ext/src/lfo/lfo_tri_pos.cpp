#include "lfo_tri_pos.h"
#include "ceammc_factory.h"

using namespace ceammc;

class LfoTriPos : public faust_lfo_tri_pos_tilde {
public:
    LfoTriPos(const PdArgs& args)
        : faust_lfo_tri_pos_tilde(args)
    {
        createInlet();
        setInitSignalValue(parsedPosArgs().floatAt(0, 0));
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

#include "fx_pitchshift.h"
#include "ceammc_factory.h"

using namespace ceammc;

class FxPitchShift : public faust_fx_pitchshift_tilde {
public:
    FxPitchShift(const PdArgs& args)
        : faust_fx_pitchshift_tilde(args)
    {
        bindPositionalArgsToProps({ gensym("@pitch") });
        createInlet();
    }

    void onInlet(size_t, const AtomListView& l) override
    {
        setProperty(gensym("@pitch"), l);
    }
};

void setup_fx_pitchshift_tilde()
{
    SoundExternalFactory<FxPitchShift> obj("fx.pitchshift~");
    obj.addMethod("reset", &FxPitchShift::m_reset);
}

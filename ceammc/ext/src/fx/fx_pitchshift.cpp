#include "fx_pitchshift.h"
#include "ceammc_faust_factory.h"

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
    FaustFactory<FxPitchShift> obj("fx.pitchshift~");

    obj.setDescription("simple pitch shifter based on 2 delay lines");
    obj.setCategory("fx");
    obj.setKeywords({ "fx", "pitchshift" });
}

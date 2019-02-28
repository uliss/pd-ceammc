#include "fx_pitchshift.h"
#include "ceammc_factory.h"

using namespace ceammc;

static t_symbol* SYM_PROP_PITCH = gensym("@pitch");

class FxPitchShift : public faust_fx_pitchshift_tilde {
public:
    FxPitchShift(const PdArgs& args)
        : faust_fx_pitchshift_tilde(args)
    {
        bindPositionalArgsToProps({ SYM_PROP_PITCH });
        createInlet();
    }

    void onInlet(size_t, const AtomList& l) override
    {
        setProperty(SYM_PROP_PITCH, l);
    }

    void m_clear(t_symbol*, const AtomList&)
    {
        dsp_->instanceClear();
    }
};

void setup_fx_pitchshift_tilde()
{
    SoundExternalFactory<FxPitchShift> obj("fx.pitchshift~");
}

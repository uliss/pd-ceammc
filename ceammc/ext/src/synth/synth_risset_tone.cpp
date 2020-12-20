#include "synth_risset_tone.h"
#include "ceammc_factory.h"

using namespace ceammc;

class SynthRissetGliss : public faust_synth_risset_tone_tilde {
public:
    SynthRissetGliss(const PdArgs& args)
        : faust_synth_risset_tone_tilde(args)
    {
        bindPositionalArgsToProps({ gensym("@centroid"), gensym("@rate") });
    }

    void onBang() final
    {
        m_reset(&s_, AtomListView {});
    }
};

void setup_synth_risset_tone_tilde()
{
    SoundExternalFactory<SynthRissetGliss> obj("synth.risset_tone~", OBJECT_FACTORY_DEFAULT);
    obj.addMethod("reset", &SynthRissetGliss::m_reset);
}

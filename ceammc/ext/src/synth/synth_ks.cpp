#include "synth_ks.h"
#include "ceammc_factory.h"
#include "synth_faust_with_freq.h"

using namespace ceammc;
using SynthKs = SynthWithFreq<faust_synth_ks_tilde>;

void setup_synth_ks_tilde()
{
    SoundExternalFactory<SynthKs> obj("synth.ks~", OBJECT_FACTORY_DEFAULT);
    obj.addMethod("note", &SynthKs::m_note);
}

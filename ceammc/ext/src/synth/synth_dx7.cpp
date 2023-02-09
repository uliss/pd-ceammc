#include "synth_dx7.h"
#include "ceammc_factory.h"
#include "synth_faust_with_freq.h"

using namespace ceammc;
using SynthDx7 = SynthWithFreq<faust_synth_dx7_tilde>;

void setup_synth_dx7_tilde()
{
    SoundExternalFactory<SynthDx7> obj("synth.dx7~", OBJECT_FACTORY_DEFAULT);
    obj.addMethod("note", &SynthDx7::m_note);

    obj.setDescription("FAUST dx7 implementation");
    obj.setCategory("synth");
    obj.setKeywords({"dx7", "synth", "fm"});
}

#include "synth_marimba.h"
#include "ceammc_factory.h"
#include "synth_faust_with_freq.h"

using namespace ceammc;
using SynthMarimba = SynthWithFreq<faust_synth_marimba_tilde>;

void setup_synth_marimba_tilde()
{
    SoundExternalFactory<SynthMarimba> obj("synth.marimba~", OBJECT_FACTORY_DEFAULT);
    obj.addMethod("note", &SynthMarimba::m_note);

    obj.setDescription("simple marimba physical model implementing a single tone bar connected to tube");
    obj.setCategory("synth");
    obj.setKeywords({"marimba"});
}

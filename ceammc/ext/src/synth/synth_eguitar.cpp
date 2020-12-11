#include "synth_eguitar.h"
#include "ceammc_factory.h"
#include "synth_faust_with_freq.h"

using namespace ceammc;
using SynthEGuitar = SynthWithFreq<faust_synth_eguitar_tilde>;

void setup_synth_eguitar_tilde()
{
    SynthEGuitar::initSymTab();

    SoundExternalFactory<SynthEGuitar> obj("synth.eguitar~", OBJECT_FACTORY_DEFAULT);
    obj.addMethod("note", &SynthEGuitar::m_note);
}

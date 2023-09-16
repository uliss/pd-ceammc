#include "synth_risset_arp.h"
#include "ceammc_factory.h"

using namespace ceammc;

void setup_synth_risset_arp_tilde()
{
    SoundExternalFactory<faust_synth_risset_arp_tilde> obj("synth.risset_arp~", OBJECT_FACTORY_DEFAULT);

    obj.setDescription("Jean Claude Risset's harmonic arpeggio effect");
    obj.setCategory("synth");
    obj.setKeywords({"risset", "arpeggio"});
}

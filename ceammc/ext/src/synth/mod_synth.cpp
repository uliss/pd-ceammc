#include "mod_synth.h"

extern "C" void setup_synth0x2erisset_tone_tilde();
extern "C" void setup_synth0x2erisset_arp_tilde();
extern "C" void setup_synth0x2ebirds_tilde();
extern "C" void setup_synth0x2emarimba_tilde();
extern "C" void setup_synth0x2eeguitar_tilde();
extern "C" void setup_synth0x2eks_tilde();
extern "C" void setup_synth0x2echurch_bell_tilde();
extern "C" void setup_synth0x2edubdub_tilde();

void ceammc_synth_setup()
{
    setup_synth0x2erisset_tone_tilde();
    setup_synth0x2erisset_arp_tilde();
    setup_synth0x2ebirds_tilde();
    setup_synth0x2emarimba_tilde();
    setup_synth0x2eks_tilde();
    setup_synth0x2eeguitar_tilde();
    setup_synth0x2echurch_bell_tilde();
    setup_synth0x2edubdub_tilde();
}

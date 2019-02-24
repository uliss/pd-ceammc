#include "mod_synth.h"
#include "ceammc_platform.h"
#include "synth_bthree.h"
#include "synth_fgrain.h"
#include "synth_rhodey.h"
#include "synth_shakers.h"
#include "synth_wurley.h"

extern "C" void setup_synth0x2emarimba_tilde();
extern "C" void setup_synth0x2eeguitar_tilde();
extern "C" void setup_synth0x2eks_tilde();
extern "C" void setup_synth0x2echurch_bell_tilde();
extern "C" void setup_synth0x2edubdub_tilde();

extern void setup_synth_birds_tilde();
extern void setup_synth_risset_arp_tilde();
extern void setup_synth_risset_tone_tilde();
extern void setup_synth_dx7_tilde();

void ceammc_synth_setup()
{
    setup_synth0x2emarimba_tilde();
    setup_synth0x2eks_tilde();
    setup_synth0x2eeguitar_tilde();
    setup_synth0x2echurch_bell_tilde();
    setup_synth0x2edubdub_tilde();

    setup_synth_bthree();
    setup_synth_fgrain();
    setup_synth_rhodey();
    setup_synth_shakers();
    setup_synth_wurley();

    setup_synth_birds_tilde();
    setup_synth_risset_arp_tilde();
    setup_synth_risset_tone_tilde();
    setup_synth_dx7_tilde();
}

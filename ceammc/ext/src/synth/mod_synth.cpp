#include "mod_synth.h"

extern "C" void setup_synth0x2erisset_tone_tilde();
extern "C" void setup_synth0x2etambura_tilde();

void ceammc_synth_setup()
{
    setup_synth0x2erisset_tone_tilde();
}

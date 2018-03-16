#include "mod_synth.h"
#include "synth_8bit.h"

extern "C" void setup_synth0x2erisset_tone_tilde();

void ceammc_synth_setup()
{
    setup_synth0x2erisset_tone_tilde();
    setup_fx_8bit();
}

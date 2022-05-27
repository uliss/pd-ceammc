#include "mod_analyze.h"
#include "an_pitchtrack.h"
#include "an_zero_tilde.h"

extern void setup_an_onset();
extern void setup_an_onset_tilde();
extern void setup_an_tempo_tilde();
extern void setup_an_rms_tilde();

void ceammc_analyze_setup()
{
    setup_an_onset();
    setup_an_onset_tilde();
    setup_an_pitchtrack();
    setup_an_rms_tilde();
    setup_an_tempo_tilde();
    setup_an_zero_tilde();
}

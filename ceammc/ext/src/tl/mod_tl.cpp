#include "mod_tl.h"

extern "C" void setup_tl0x2ecue();
extern "C" void setup_tl0x2etransport();

void setup_tl_bang();
void setup_tl_timeline();
void setup_tl_toggle();
void setup_tl_cue();

void ceammc_tl_setup()
{
    setup_tl_bang();
    setup_tl_timeline();
    setup_tl_toggle();
    setup_tl_cue();

    setup_tl0x2etransport();
    setup_tl0x2ecue();
}

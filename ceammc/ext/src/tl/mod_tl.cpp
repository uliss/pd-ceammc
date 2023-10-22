#include "mod_tl.h"

void setup_tl_bang();
void setup_tl_cue();
void setup_tl_events();
void setup_tl_timeline();
void setup_tl_toggle();
void setup_tl_transport();

void ceammc_tl_setup()
{
    setup_tl_bang();
    setup_tl_cue();
    setup_tl_events();
    setup_tl_timeline();
    setup_tl_toggle();
    setup_tl_transport();
}

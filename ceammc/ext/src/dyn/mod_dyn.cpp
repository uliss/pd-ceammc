#include "mod_dyn.h"

extern void setup_dyn_comp2_tilde();
extern void setup_dyn_comp_tilde();
extern void setup_dyn_gate2_tilde();
extern void setup_dyn_gate_tilde();
extern void setup_dyn_limit2_tilde();
extern void setup_dyn_limit_tilde();
extern void setup_dyn_softclip_tilde();

void ceammc_dyn_setup()
{
    setup_dyn_comp2_tilde();
    setup_dyn_comp_tilde();
    setup_dyn_gate2_tilde();
    setup_dyn_gate_tilde();
    setup_dyn_limit2_tilde();
    setup_dyn_limit_tilde();
    setup_dyn_softclip_tilde();
}

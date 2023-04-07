#include "mod_dyn.h"

void setup_dyn_comp2_tilde();
void setup_dyn_comp_peak_tilde();
void setup_dyn_comp_peak2_tilde();
void setup_dyn_comp_tilde();
void setup_dyn_gate2_tilde();
void setup_dyn_gate_tilde();
void setup_dyn_limit2_tilde();
void setup_dyn_limit_tilde();
void setup_dyn_softclip_tilde();

void ceammc_dyn_setup()
{
    setup_dyn_comp2_tilde();
    setup_dyn_comp_peak_tilde();
    setup_dyn_comp_peak2_tilde();
    setup_dyn_comp_tilde();
    setup_dyn_gate2_tilde();
    setup_dyn_gate_tilde();
    setup_dyn_limit2_tilde();
    setup_dyn_limit_tilde();
    setup_dyn_softclip_tilde();
}

#include "mod_exp.h"

extern "C" void setup_exp0x2eaproperty();
extern "C" void setup_exp0x2eclass();
extern "C" void setup_exp0x2einstance();
extern "C" void setup_exp0x2emethod();
extern "C" void setup_exp0x2eparent();
extern "C" void setup_exp0x2epatchdata();
extern "C" void setup_exp0x2erecursion();
extern "C" void setup_exp0x2esignal_tilde();
extern "C" void setup_exp0x2ethis();
extern "C" void setup_exp0x2evinstance();
extern "C" void setup_ui0x2eecanvas();

void ceammc_exp_setup() {
     setup_exp0x2eaproperty();
     setup_exp0x2eclass();
     setup_exp0x2einstance();
     setup_exp0x2emethod();
     setup_exp0x2eparent();
     setup_exp0x2epatchdata();
     setup_exp0x2erecursion();
     setup_exp0x2esignal_tilde();
     setup_exp0x2ethis();
     setup_exp0x2evinstance();
     setup_ui0x2eecanvas();
}

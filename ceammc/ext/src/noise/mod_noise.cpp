#include "mod_noise.h"
#include "m_pd.h"

extern "C" void setup_noise0x2elfreq_tilde();
extern "C" void setup_noise0x2epink_tilde();
extern "C" void setup_noise0x2ewhite_tilde();
extern "C" void setup_noise0x2ebaker();
extern "C" void setup_noise0x2eclifford();
extern "C" void setup_noise0x2ecollatz();
extern "C" void setup_noise0x2eduffing();
extern "C" void setup_noise0x2eginger();
extern "C" void setup_noise0x2ehenon();
extern "C" void setup_noise0x2ehenon_heilles();
extern "C" void setup_noise0x2ehenon_phase();
extern "C" void setup_noise0x2ehenonf();
extern "C" void setup_noise0x2eikeda();
extern "C" void setup_noise0x2ejong();
extern "C" void setup_noise0x2elogistic();
extern "C" void setup_noise0x2elorenz();
extern "C" void setup_noise0x2elyapunov();
extern "C" void setup_noise0x2enavier_stokes();
extern "C" void setup_noise0x2erossler();
extern "C" void setup_noise0x2estein();
extern "C" void setup_noise0x2etorus();
extern "C" void setup_noise0x2everhulst();

void ceammc_noise_setup() {
     setup_noise0x2elfreq_tilde();
     setup_noise0x2epink_tilde();
     setup_noise0x2ewhite_tilde();
     setup_noise0x2ebaker();
     setup_noise0x2eclifford();
     setup_noise0x2ecollatz();
     setup_noise0x2eduffing();
     setup_noise0x2eginger();
     setup_noise0x2ehenon();
     setup_noise0x2ehenon_heilles();
     setup_noise0x2ehenon_phase();
     setup_noise0x2ehenonf();
     setup_noise0x2eikeda();
     setup_noise0x2ejong();
     setup_noise0x2elogistic();
     setup_noise0x2elorenz();
     setup_noise0x2elyapunov();
     setup_noise0x2enavier_stokes();
     setup_noise0x2erossler();
     setup_noise0x2estein();
     setup_noise0x2etorus();
     setup_noise0x2everhulst();

     post("CEAMMC: using code from of A-Chaos library, (c) 2004 André Sier");
}

#include "mod_flt.h"

extern "C" void setup_flt0x2ebiquad_tilde();
extern "C" void setup_flt0x2ebpf12_tilde();
extern "C" void setup_flt0x2ebpf24_tilde();
extern "C" void setup_flt0x2ec_bpf_tilde();
extern "C" void setup_flt0x2ec_highshelf_tilde();
extern "C" void setup_flt0x2ec_hpf_tilde();
extern "C" void setup_flt0x2ec_lowshelf_tilde();
extern "C" void setup_flt0x2ec_lpf_tilde();
extern "C" void setup_flt0x2ec_notch_tilde();
extern "C" void setup_flt0x2ec_peak_tilde();
extern "C" void setup_flt0x2edcblock2_tilde();
extern "C" void setup_flt0x2edcblock_tilde();
extern "C" void setup_flt0x2eeq_peak_tilde();
extern "C" void setup_flt0x2ehighshelf_tilde();
extern "C" void setup_flt0x2ehpf12_tilde();
extern "C" void setup_flt0x2ehpf24_tilde();
extern "C" void setup_flt0x2elowshelf_tilde();
extern "C" void setup_flt0x2elpf12_tilde();
extern "C" void setup_flt0x2elpf24_tilde();
extern "C" void setup_flt0x2enotch_tilde();
extern "C" void setup_flt0x2eresonbp_tilde();

void ceammc_flt_setup() {
     setup_flt0x2ebiquad_tilde();
     setup_flt0x2ebpf12_tilde();
     setup_flt0x2ebpf24_tilde();
     setup_flt0x2ec_bpf_tilde();
     setup_flt0x2ec_highshelf_tilde();
     setup_flt0x2ec_hpf_tilde();
     setup_flt0x2ec_lowshelf_tilde();
     setup_flt0x2ec_lpf_tilde();
     setup_flt0x2ec_notch_tilde();
     setup_flt0x2ec_peak_tilde();
     setup_flt0x2edcblock2_tilde();
     setup_flt0x2edcblock_tilde();
     setup_flt0x2eeq_peak_tilde();
     setup_flt0x2ehighshelf_tilde();
     setup_flt0x2ehpf12_tilde();
     setup_flt0x2ehpf24_tilde();
     setup_flt0x2elowshelf_tilde();
     setup_flt0x2elpf12_tilde();
     setup_flt0x2elpf24_tilde();
     setup_flt0x2enotch_tilde();
     setup_flt0x2eresonbp_tilde();
}

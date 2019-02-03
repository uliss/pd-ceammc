#include "mod_flt.h"

extern "C" void setup_flt0x2ebiquad_tilde();
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
extern "C" void setup_flt0x2eeq_peak_cq_tilde();
extern "C" void setup_flt0x2ehpf12_tilde();
extern "C" void setup_flt0x2ehpf24_tilde();
extern "C" void setup_flt0x2elpf12_tilde();
extern "C" void setup_flt0x2elpf24_tilde();
extern "C" void setup_flt0x2enotch_tilde();

extern void setup_flt_bpf12_tilde();
extern void setup_flt_eq10_tilde();
extern void setup_flt_eq_peak_tilde();
extern void setup_flt_highshelf_tilde();
extern void setup_flt_lowshelf_tilde();
extern void setup_flt_moog_vcf_tilde();
extern void setup_flt_resonbp_tilde();
extern void setup_flt_median();

void ceammc_flt_setup()
{
    setup_flt0x2ebiquad_tilde();
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
    setup_flt0x2eeq_peak_cq_tilde();
    setup_flt0x2ehpf12_tilde();
    setup_flt0x2ehpf24_tilde();
    setup_flt0x2elpf12_tilde();
    setup_flt0x2elpf24_tilde();
    setup_flt0x2enotch_tilde();

    setup_flt_bpf12_tilde();
    setup_flt_eq10_tilde();
    setup_flt_eq_peak_tilde();
    setup_flt_highshelf_tilde();
    setup_flt_lowshelf_tilde();
    setup_flt_moog_vcf_tilde();
    setup_flt_resonbp_tilde();
    setup_flt_median();
}

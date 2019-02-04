#include "mod_flt.h"

extern "C" void setup_flt0x2ebiquad_tilde();
extern "C" void setup_flt0x2edcblock2_tilde();
extern "C" void setup_flt0x2edcblock_tilde();
extern "C" void setup_flt0x2enotch_tilde();

extern void setup_flt_bpf12_tilde();
extern void setup_flt_bpf24_tilde();
extern void setup_flt_c_bpf_tilde();
extern void setup_flt_c_highshelf_tilde();
extern void setup_flt_c_hpf_tilde();
extern void setup_flt_c_lowshelf_tilde();
extern void setup_flt_c_lpf_tilde();
extern void setup_flt_c_notch_tilde();
extern void setup_flt_c_peak_tilde();
extern void setup_flt_eq10_tilde();
extern void setup_flt_eq_peak_tilde();
extern void setup_flt_eq_peak_cq_tilde();
extern void setup_flt_highshelf_tilde();
extern void setup_flt_hpf12_tilde();
extern void setup_flt_hpf24_tilde();
extern void setup_flt_lowshelf_tilde();
extern void setup_flt_lpf12_tilde();
extern void setup_flt_lpf24_tilde();
extern void setup_flt_moog_vcf_tilde();
extern void setup_flt_resonbp_tilde();
extern void setup_flt_median();

void ceammc_flt_setup()
{
    setup_flt0x2ebiquad_tilde();
    setup_flt0x2edcblock2_tilde();
    setup_flt0x2edcblock_tilde();
    setup_flt0x2enotch_tilde();

    setup_flt_bpf12_tilde();
    setup_flt_bpf24_tilde();
    setup_flt_c_bpf_tilde();
    setup_flt_c_highshelf_tilde();
    setup_flt_c_hpf_tilde();
    setup_flt_c_lowshelf_tilde();
    setup_flt_c_lpf_tilde();
    setup_flt_c_notch_tilde();
    setup_flt_c_peak_tilde();
    setup_flt_eq10_tilde();
    setup_flt_eq_peak_tilde();
    setup_flt_eq_peak_cq_tilde();
    setup_flt_highshelf_tilde();
    setup_flt_hpf12_tilde();
    setup_flt_hpf24_tilde();
    setup_flt_lowshelf_tilde();
    setup_flt_lpf12_tilde();
    setup_flt_lpf24_tilde();
    setup_flt_moog_vcf_tilde();
    setup_flt_resonbp_tilde();
    setup_flt_median();
}

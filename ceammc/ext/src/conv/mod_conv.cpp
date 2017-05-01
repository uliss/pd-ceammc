#include "mod_conv.h"

extern "C" void setup_conv0x2esec2str();
extern "C" void setup_conv0x2estr2sec();
extern "C" void setup_conv0x2ebpf2array();
extern "C" void setup_conv0x2ebpf2env();
extern "C" void setup_conv0x2ebpf2list();
extern "C" void setup_conv0x2ebpf2vline();
extern "C" void setup_conv0x2eenv2bpf();
extern "C" void setup_conv0x2elin2curve();
extern "C" void setup_conv0x2elin2exp();
extern "C" void setup_conv0x2elin2lin();
extern "C" void setup_conv0x2elist2bpf();
extern "C" void setup_conv0x2evline2bpf();

extern "C" void setup_conv0x2ecc2amp();

extern "C" void setup_conv0x2esamp2sec();
extern "C" void setup_conv0x2esec2samp();
extern "C" void setup_conv0x2ebpm2ms();
extern "C" void setup_conv0x2ebpm2sec();
extern "C" void setup_conv0x2ebpm2hz();

void ceammc_conv_setup()
{
    setup_conv0x2esec2str();
    setup_conv0x2estr2sec();
    setup_conv0x2ebpf2array();
    setup_conv0x2ebpf2env();
    setup_conv0x2ebpf2list();
    setup_conv0x2ebpf2vline();
    setup_conv0x2eenv2bpf();
    setup_conv0x2elist2bpf();
    setup_conv0x2evline2bpf();

    setup_conv0x2elin2curve();
    setup_conv0x2elin2exp();
    setup_conv0x2elin2lin();

    setup_conv0x2ecc2amp();

    setup_conv0x2esamp2sec();
    setup_conv0x2esec2samp();
    setup_conv0x2ebpm2ms();
    setup_conv0x2ebpm2sec();
    setup_conv0x2ebpm2hz();
}

#include "mod_conv.h"

extern "C" void setup_conv0x2esec2str();
extern "C" void setup_conv0x2ebpf2array();
extern "C" void setup_conv0x2ebpf2env();
extern "C" void setup_conv0x2ebpf2list();
extern "C" void setup_conv0x2ebpf2vline();
extern "C" void setup_conv0x2eenv2bpf();
extern "C" void setup_conv0x2elin2lin();
extern "C" void setup_conv0x2elist2bpf();
extern "C" void setup_conv0x2evline2bpf();

void ceammc_conv_setup() {
     setup_conv0x2esec2str();
     setup_conv0x2ebpf2array();
     setup_conv0x2ebpf2env();
     setup_conv0x2ebpf2list();
     setup_conv0x2ebpf2vline();
     setup_conv0x2eenv2bpf();
     setup_conv0x2elin2lin();
     setup_conv0x2elist2bpf();
     setup_conv0x2evline2bpf();
}

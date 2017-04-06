#include "mod_ui.h"

extern "C" void setup_ui0x2ebpfunc();
extern "C" void setup_ui0x2edisplay();
extern "C" void setup_ui0x2ekeyboard();
extern "C" void setup_ui0x2eknob();
extern "C" void setup_ui0x2elink();
extern "C" void setup_ui0x2escope_tilde();
extern "C" void setup_ui0x2eslider2d();
extern "C" void setup_ui0x2esliders();
extern "C" void setup_ui0x2espectroscope_tilde();

void ceammc_ui_setup() {
     setup_ui0x2ebpfunc();
     setup_ui0x2edisplay();
     setup_ui0x2ekeyboard();
     setup_ui0x2eknob();
     setup_ui0x2elink();
     setup_ui0x2escope_tilde();
     setup_ui0x2eslider2d();
     setup_ui0x2esliders();
     setup_ui0x2espectroscope_tilde();
}

#include "mod_base.h"

extern "C" void m_setup();
extern "C" void msg_setup();

void ceammc_base_setup() {
     m_setup();
     msg_setup();
}

#include "mod_system.h"

void setup_system_getenv();
extern "C" void setup_system0x2ehostname();
extern "C" void setup_system0x2ememsize();
extern "C" void setup_system0x2ememused();
extern "C" void setup_system0x2escreen_size();

void setup_system_colorpanel();
void setup_system_cursor();
void setup_system_shell();

void ceammc_system_setup()
{
    setup_system_getenv();
    setup_system0x2ehostname();
    setup_system0x2ememsize();
    setup_system0x2ememused();
    setup_system0x2escreen_size();

    setup_system_colorpanel();
    setup_system_cursor();
    setup_system_shell();
}

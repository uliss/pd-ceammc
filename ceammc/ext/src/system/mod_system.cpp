#include "mod_system.h"

void setup_system_colorpanel();
void setup_system_cursor();
void setup_system_exec();
void setup_system_getenv();
void setup_system_hostname();
void setup_system_memsize();
void setup_system_memused();
void setup_system_screen_size();

void ceammc_system_setup()
{
    setup_system_colorpanel();
    setup_system_cursor();
    setup_system_exec();
    setup_system_getenv();
    setup_system_hostname();
    setup_system_memsize();
    setup_system_memused();
    setup_system_screen_size();
}

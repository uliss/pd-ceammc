#include "mod_symbol.h"

extern "C" void setup_symbol0x2eends_with();
extern "C" void setup_symbol0x2eequal();
extern "C" void setup_symbol0x2elength();
extern "C" void setup_symbol0x2enum_compare();

void ceammc_symbol_setup()
{
    setup_symbol0x2eends_with();
    setup_symbol0x2eequal();
    setup_symbol0x2elength();
    setup_symbol0x2enum_compare();
}

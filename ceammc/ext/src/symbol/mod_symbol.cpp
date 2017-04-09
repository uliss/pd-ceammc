#include "mod_symbol.h"

extern "C" void setup_string0x2ecompare();
extern "C" void setup_symbol0x2eends_with();
extern "C" void setup_symbol0x2elength();

void ceammc_symbol_setup()
{
    setup_symbol0x2eends_with();

#ifdef WITH_GLIB
    setup_string0x2ecompare();
    setup_symbol0x2elength();
#endif
}

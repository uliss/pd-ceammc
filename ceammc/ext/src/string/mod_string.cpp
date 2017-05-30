#include "mod_string.h"
#include "string_format.h"
#include "string_str.h"
#include "string_to_symbol.h"

void ceammc_string_setup()
{
    string_setup();
    setup_string0x2eto_symbol();
    setup_string0x2eformat();
}

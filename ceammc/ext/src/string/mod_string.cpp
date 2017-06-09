#include "mod_string.h"
#include "string_format.h"
#include "string_join.h"
#include "string_remove.h"
#include "string_split.h"
#include "string_str.h"
#include "string_to_symbol.h"

void ceammc_string_setup()
{
    string_setup();
    setup_string0x2eto_symbol();
    setup_string0x2eformat();
    setup_string0x2ejoin();
    setup_string0x2esplit();
    setup_string0x2eremove();
}

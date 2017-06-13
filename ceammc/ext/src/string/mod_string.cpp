#include "mod_string.h"
#include "string_contains.h"
#include "string_ends_with.h"
#include "string_equal.h"
#include "string_format.h"
#include "string_join.h"
#include "string_length.h"
#include "string_remove.h"
#include "string_replace.h"
#include "string_split.h"
#include "string_starts_with.h"
#include "string_str.h"
#include "string_substr.h"
#include "string_to_symbol.h"

void ceammc_string_setup()
{
    setup_string0x2econtains();
    setup_string0x2eends_with();
    setup_string0x2eequal();
    setup_string0x2eformat();
    setup_string0x2ejoin();
    setup_string0x2elength();
    setup_string0x2eremove();
    setup_string0x2ereplace();
    setup_string0x2esplit();
    setup_string0x2estarts_with();
    setup_string0x2esubstr();
    setup_string0x2eto_symbol();
    string_setup();
}

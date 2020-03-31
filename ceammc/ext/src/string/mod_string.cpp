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

void setup_string_match();

void ceammc_string_setup()
{
    setup_string0x2eformat();
    setup_string0x2esplit();
    setup_string0x2esubstr();
    setup_string0x2eto_symbol();
    setup_string_contains();
    setup_string_ends_with();
    setup_string_equal();
    setup_string_join();
    setup_string_length();
    setup_string_match();
    setup_string_remove();
    setup_string_replace();
    setup_string_starts_with();
    setup_string_str();
}

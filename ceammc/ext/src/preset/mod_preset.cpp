#include "mod_preset.h"
#include "preset_float.h"
#include "preset_storage.h"
#include "preset_symbol.h"

void ceammc_preset_setup()
{
    setup_preset_float();
    setup_preset_storage();
    setup_preset_symbol();
}

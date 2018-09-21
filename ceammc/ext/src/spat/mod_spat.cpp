#include "mod_spat.h"
#include "pan_cosine.h"
#include "pan_linear.h"
#include "pan_linsig.h"
#include "pan_spread.h"
#include "pan_sqrt.h"

extern "C" void setup_spat0x2epan8_tilde();
extern "C" void setup_spat0x2epan4_tilde();

void ceammc_spat_setup()
{
    setup_spat0x2epan8_tilde();
    setup_spat0x2epan4_tilde();
    setup_pan_linear();
    setup_pan_linsig();
    setup_pan_cosine();
    setup_pan_sqrt();
    setup_pan_spread();
}

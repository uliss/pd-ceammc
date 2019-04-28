#include "mod_spat.h"
#include "pan_cosine.h"
#include "pan_linear.h"
#include "pan_linsig.h"
#include "pan_spread.h"
#include "pan_sqrt.h"

extern void setup_spat_pan8_tilde();
extern void setup_spat_pan4_tilde();
extern void setup_spat_hoa_encoder();
extern void setup_spat_hoa_rotate();
extern void setup_spat_hoa_decoder();

void ceammc_spat_setup()
{
    setup_spat_pan8_tilde();
    setup_spat_pan4_tilde();
    setup_pan_linear();
    setup_pan_linsig();
    setup_pan_cosine();
    setup_pan_sqrt();
    setup_pan_spread();
    setup_spat_hoa_encoder();
    setup_spat_hoa_rotate();
    setup_spat_hoa_decoder();
}

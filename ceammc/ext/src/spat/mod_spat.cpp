#include "mod_spat.h"
#include "pan_cosine.h"
#include "pan_linear.h"
#include "pan_linsig.h"
#include "pan_spread.h"
#include "pan_sqrt.h"

extern void setup_spat_hoa_2d_decoder();
extern void setup_spat_hoa_2d_encoder();
extern void setup_spat_hoa_2d_exchanger();
extern void setup_spat_hoa_2d_map();
extern void setup_spat_hoa_2d_optim();
extern void setup_spat_hoa_2d_projector();
extern void setup_spat_hoa_2d_recomposer();
extern void setup_spat_hoa_2d_rotate();
extern void setup_spat_hoa_2d_scope();
extern void setup_spat_hoa_2d_wider();
extern void setup_spat_hoa_3d_decoder();
extern void setup_spat_hoa_3d_encoder();
extern void setup_spat_hoa_3d_exchanger();
extern void setup_spat_hoa_3d_optim();
extern void setup_spat_hoa_3d_scope();
extern void setup_spat_hoa_3d_wider();
extern void setup_spat_hoa_connections();
extern void setup_spat_hoa_process();
extern void setup_spat_hoa_process_inlet();
extern void setup_spat_hoa_process_props();
extern void setup_spat_pan4_tilde();
extern void setup_spat_pan8_tilde();
extern void setup_spat_zita6x8_tilde();
extern void setup_spat_zita8_tilde();

void ceammc_spat_setup()
{
    setup_pan_cosine();
    setup_pan_linear();
    setup_pan_linsig();
    setup_pan_spread();
    setup_pan_sqrt();
    setup_spat_hoa_2d_decoder();
    setup_spat_hoa_2d_encoder();
    setup_spat_hoa_2d_exchanger();
    setup_spat_hoa_2d_map();
    setup_spat_hoa_2d_optim();
    setup_spat_hoa_2d_projector();
    setup_spat_hoa_2d_recomposer();
    setup_spat_hoa_2d_rotate();
    setup_spat_hoa_2d_scope();
    setup_spat_hoa_2d_wider();
    setup_spat_hoa_3d_decoder();
    setup_spat_hoa_3d_encoder();
    setup_spat_hoa_3d_exchanger();
    setup_spat_hoa_3d_optim();
    setup_spat_hoa_3d_scope();
    setup_spat_hoa_3d_wider();
    setup_spat_hoa_connections();
    setup_spat_hoa_process();
    setup_spat_hoa_process_inlet();
    setup_spat_hoa_process_props();
    setup_spat_pan4_tilde();
    setup_spat_pan8_tilde();
    setup_spat_zita6x8_tilde();
    setup_spat_zita8_tilde();
}

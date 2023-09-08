#include "mod_spat.h"
#include "pan_cosine.h"
#include "pan_linear.h"
#include "pan_linsig.h"
#include "pan_spread.h"
#include "pan_sqrt.h"

extern void setup_spat_hoa_2d_wider();
extern void setup_spat_hoa_3d_wider();
extern void setup_spat_hoa_connections();
extern void setup_spat_hoa_decoder_2d();
extern void setup_spat_hoa_encoder();
extern void setup_spat_hoa_encoder_3d();
extern void setup_spat_hoa_exchanger_2d();
extern void setup_spat_hoa_exchanger_3d();
extern void setup_spat_hoa_map_2d();
extern void setup_spat_hoa_optim_2d();
extern void setup_spat_hoa_optim_3d();
extern void setup_spat_hoa_process();
extern void setup_spat_hoa_process_inlet();
extern void setup_spat_hoa_process_props();
extern void setup_spat_hoa_projector();
extern void setup_spat_hoa_recomposer_2d();
extern void setup_spat_hoa_rotate_2d();
extern void setup_spat_hoa_scope_2d();
extern void setup_spat_hoa_scope_3d();
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
    setup_spat_hoa_2d_wider();
    setup_spat_hoa_3d_wider();
    setup_spat_hoa_connections();
    setup_spat_hoa_decoder_2d();
    setup_spat_hoa_encoder();
    setup_spat_hoa_encoder_3d();
    setup_spat_hoa_exchanger_2d();
    setup_spat_hoa_exchanger_3d();
    setup_spat_hoa_map_2d();
    setup_spat_hoa_optim_2d();
    setup_spat_hoa_optim_3d();
    setup_spat_hoa_process();
    setup_spat_hoa_process_inlet();
    setup_spat_hoa_process_props();
    setup_spat_hoa_projector();
    setup_spat_hoa_recomposer_2d();
    setup_spat_hoa_rotate_2d();
    setup_spat_hoa_scope_2d();
    setup_spat_hoa_scope_3d();
    setup_spat_pan4_tilde();
    setup_spat_pan8_tilde();
    setup_spat_zita6x8_tilde();
    setup_spat_zita8_tilde();
}

#include "mod_fx.h"
#include "fx_looper.h"

extern "C" void setup_fx0x2edistortion_tilde();
extern "C" void setup_fx0x2edrone_box_tilde();
extern "C" void setup_fx0x2efreqshift_tilde();
extern "C" void setup_fx0x2egranulator_tilde();
extern "C" void setup_fx0x2ewahwah_tilde();

extern void setup_fx_chorus_tilde();
extern void setup_fx_echo_tilde();
extern void setup_fx_flanger_tilde();
extern void setup_fx_freeverb2_tilde();
extern void setup_fx_freeverb_tilde();
extern void setup_fx_greyhole_tilde();
extern void setup_fx_pitchshift_tilde();
extern void setup_fx_sdelay_tilde();
extern void setup_fx_zita_rev1_tilde();

void ceammc_fx_setup()
{
    setup_fx0x2edistortion_tilde();
    setup_fx0x2edrone_box_tilde();
    setup_fx0x2efreqshift_tilde();
    setup_fx0x2egranulator_tilde();
    setup_fx0x2ewahwah_tilde();

    setup_fx_chorus_tilde();
    setup_fx_echo_tilde();
    setup_fx_flanger_tilde();
    setup_fx_freeverb2_tilde();
    setup_fx_freeverb_tilde();
    setup_fx_greyhole_tilde();
    setup_fx_looper();
    setup_fx_pitchshift_tilde();
    setup_fx_sdelay_tilde();
    setup_fx_zita_rev1_tilde();
}

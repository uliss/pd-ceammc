#include "mod_fx.h"
#include "fx_looper.h"

extern "C" void setup_fx0x2edistortion_tilde();
extern "C" void setup_fx0x2edrone_box_tilde();
extern "C" void setup_fx0x2eecho_tilde();
extern "C" void setup_fx0x2efreeverb2_tilde();
extern "C" void setup_fx0x2efreeverb_tilde();
extern "C" void setup_fx0x2efreqshift_tilde();
extern "C" void setup_fx0x2egranulator_tilde();
extern "C" void setup_fx0x2epitchshift_tilde();
extern "C" void setup_fx0x2ewahwah_tilde();
extern "C" void setup_fx0x2ezita_rev1_tilde();
extern "C" void setup_fx0x2esdelay_tilde();

extern void setup_fx_flanger_tilde();
extern void setup_fx_greyhole_tilde();

void ceammc_fx_setup()
{
    setup_fx0x2edistortion_tilde();
    setup_fx0x2edrone_box_tilde();
    setup_fx0x2eecho_tilde();
    setup_fx0x2efreeverb2_tilde();
    setup_fx0x2efreeverb_tilde();
    setup_fx0x2efreqshift_tilde();
    setup_fx0x2egranulator_tilde();
    setup_fx0x2epitchshift_tilde();
    setup_fx0x2ewahwah_tilde();
    setup_fx0x2ezita_rev1_tilde();
    setup_fx0x2esdelay_tilde();

    setup_fx_flanger_tilde();
    setup_fx_greyhole_tilde();
    setup_fx_looper();
}

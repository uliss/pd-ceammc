#include "mod_conv.h"
#include "conv_amp2dbfs.h"
#include "conv_dbfs2amp.h"
#include "conv_list2props.h"
#include "conv_midi2freq.h"
#include "conv_pitch2midi.h"

extern "C" void setup_conv0x2esec2str();
extern "C" void setup_conv0x2estr2sec();
extern "C" void setup_conv0x2elin2curve();
extern "C" void setup_conv0x2elin2exp();
extern "C" void setup_conv0x2elin2lin();

extern "C" void setup_conv0x2ecc2amp();

extern "C" void setup_conv0x2esamp2sec();
extern "C" void setup_conv0x2esec2samp();
extern "C" void setup_conv0x2ebpm2ms();
extern "C" void setup_conv0x2ebpm2sec();
extern "C" void setup_conv0x2ebpm2hz();

extern void setup_conv_angles();

void ceammc_conv_setup()
{
    setup_conv0x2esec2str();
    setup_conv0x2estr2sec();

    setup_conv0x2elin2curve();
    setup_conv0x2elin2exp();
    setup_conv0x2elin2lin();

    setup_conv0x2ecc2amp();

    setup_conv0x2esamp2sec();
    setup_conv0x2esec2samp();
    setup_conv0x2ebpm2ms();
    setup_conv0x2ebpm2sec();
    setup_conv0x2ebpm2hz();

    setup_conv0x2epitch2midi();
    setup_conv_amp2dbfs();
    setup_conv_dbfs2amp();

    setup_conv_midi2freq();
    setup_list_to_props();

    setup_conv_angles();
}

#include "mod_proto.h"
#include "proto_firmata.h"
#include "proto_hui.h"
#include "proto_ltc_in_tilde.h"
#include "proto_ltcout_tilde.h"
#include "proto_rust.hpp"
#include "proto_sp_alpaca.h"
#include "proto_whammy.h"

void setup_proto_arturia_minilab();
void setup_proto_feelworld();
void setup_proto_inscore();
void setup_proto_midi();
void setup_proto_midi_casio();
void setup_proto_midi_cc();
void setup_proto_midi_sysex();
void setup_proto_moppy();
void setup_proto_morse();
void setup_proto_mpv();
void setup_proto_vlc();
void setup_proto_xtouch_ext();
void setup_proto_obs_client();

void ceammc_proto_setup()
{
    ceammc_proto_log_init();

    setup_proto_arturia_minilab();
    setup_proto_feelworld();
    setup_proto_firmata();
    setup_proto_hui();
    setup_proto_inscore();
    setup_proto_ltcin_tilde();
    setup_proto_ltcout_tilde();
    setup_proto_midi();
    setup_proto_midi_casio();
    setup_proto_midi_cc();
    setup_proto_midi_sysex();
    setup_proto_moppy();
    setup_proto_morse();
    setup_proto_mpv();
    setup_proto_obs_client();
    setup_proto_sp_alpaca();
    setup_proto_vlc();
    setup_proto_whammy();
    setup_proto_xtouch_ext();
}

#include "mod_proto.h"
#include "proto_firmata.h"
#include "proto_hui.h"
#include "proto_sp_alpaca.h"

void setup_proto_midi();
void setup_proto_midi_sysex();
void setup_proto_mpv();
void setup_proto_xtouch_ext();

void ceammc_proto_setup()
{
    setup_proto_firmata();
    setup_proto_hui();
    setup_proto_midi();
    setup_proto_midi_sysex();
    setup_proto_mpv();
    setup_proto_sp_alpaca();
    setup_proto_xtouch_ext();
}

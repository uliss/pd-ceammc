#include "mod_midi.h"
#include "midi_cc.h"
#include "midi_clock.h"
#include "midi_ctl2str.h"
#include "midi_event.h"
#include "midi_event_to_ctl.h"
#include "midi_file.h"
#include "midi_key2str.h"
#include "midi_prg2str.h"
#include "midi_sysex.h"
#include "midi_track.h"
#include "midi_tuning.h"
#include "midi_velocity_ramp.h"

void ceammc_midi_setup()
{
    setup_midi_cc();
    setup_midi_clock();
    setup_midi_ctl2str();
    setup_midi_event();
    setup_midi_event2ctl();
    setup_midi_file();
    setup_midi_key2str();
    setup_midi_prg2str();
    setup_midi_sysex();
    setup_midi_track();
    setup_midi_tuning();
    setup_midi_vel_ramp();
}

#include "mod_music.h"

#include "duration_to_time.h"
#include "rhythm_euclid.h"
#include "rhythm_hexbeat.h"
#include "voice_to_midi.h"

void setup_rhythm_lsystem();

void ceammc_music_setup()
{
    setup_music_duration2time();
    setup_music_voice2midi();
    setup_rhythm_euclid();
    setup_rhythm_hexbeat();
    setup_rhythm_lsystem();
}

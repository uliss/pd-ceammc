#include "mod_music.h"

#include "duration_to_time.h"
#include "voice_to_midi.h"

void ceammc_music_setup()
{
    setup_music_duration2time();
    setup_music_voice2midi();
}

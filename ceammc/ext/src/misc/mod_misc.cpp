#include "mod_misc.h"
#include "ceammc_config.h"

#ifdef CEAMMC_HAVE_TTS_FLITE
#include "speech_flite.h"
#include "speech_flite_tilde.h"
#endif

#ifdef WITH_MODPLUG
#include "misc_modplug.h"
#endif

#include "fluid.h"

extern void setup_live_capture_tilde();

void ceammc_misc_setup()
{
    setup_live_capture_tilde();

#ifdef WITH_MODPLUG
    setup_misc_modplug_tilde();
#endif

#ifdef CEAMMC_HAVE_FLUIDSYNTH
    setup_misc_fluid();
#endif

#ifdef CEAMMC_HAVE_TTS_FLITE
    setup_misc_speech_flite();
    setup_misc_speech_filte_tilde();
#endif
}

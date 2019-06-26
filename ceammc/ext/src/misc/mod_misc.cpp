#include "mod_misc.h"
#include "ceammc_config.h"
#include "speech_flite.h"

#ifdef WITH_MODPLUG
#include "misc_modplug.h"
#endif

#include "fluid.h"

extern void setup_live_capture_tilde();

void ceammc_misc_setup()
{
    setup_live_capture_tilde();
    setup_misc_speech_flite();

#ifdef WITH_MODPLUG
    setup_misc0x2emodplug_tilde();
#endif

#ifdef CEAMMC_HAVE_FLUIDSYNTH
    setup_misc_fluid();
#endif
}

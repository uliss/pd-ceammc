#include "mod_misc.h"
#include "ceammc_config.h"

#ifdef CEAMMC_HAVE_TTS_FLITE
#include "speech_flite.h"
#include "speech_flite_tilde.h"
#endif

#ifdef WITH_RHVOICE
#include "speech_rhvoice_tilde.h"
#else
extern "C" void setup_speech_rhvoice_tilde();
#endif

#ifdef WITH_MODPLUG
#include "misc_modplug.h"
#endif

#include "aubio_pitchshift_tilde.h"
#include "fluid.h"
#include "misc_ltcout_tilde.h"
#include "risset_glissando.h"

extern void setup_live_capture_tilde();
extern void setup_misc_qrcode();
extern void setup_misc_sfizz_tilde();

void ceammc_misc_setup()
{
    setup_aubio_pitchshift_tilde();
    setup_live_capture_tilde();
    setup_misc_qrcode();
    setup_misc_risset_glissando();
    setup_speech_rhvoice_tilde();

#ifdef WITH_MODPLUG
    setup_misc_modplug_tilde();
#endif

#ifdef CEAMMC_HAVE_FLUIDSYNTH
    setup_misc_fluid();
#endif

#ifdef WITH_SFIZZ
    setup_misc_sfizz_tilde();
#endif

#ifdef CEAMMC_HAVE_TTS_FLITE
    setup_misc_speech_flite();
    setup_misc_speech_filte_tilde();
#endif

    setup_misc_ltcout_tilde();
}

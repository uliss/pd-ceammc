#include "mod_synth.h"
#include "synth_bthree.h"
#include "synth_fgrain.h"
#include "synth_rhodey.h"
#include "synth_shakers.h"
#include "synth_sitar.h"
#include "synth_wurley.h"

void setup_synth_birds_tilde();
void setup_synth_church_bell_tilde();
void setup_synth_clap_tilde();
void setup_synth_dubdub_tilde();
void setup_synth_dx7_tilde();
void setup_synth_eguitar_tilde();
void setup_synth_english_bell_tilde();
void setup_synth_french_bell_tilde();
void setup_synth_german_bell_tilde();
void setup_synth_glass_harm_tilde();
void setup_synth_glitch();
void setup_synth_harpsichord_tilde();
void setup_synth_hat_tilde();
void setup_synth_kick2_tilde();
void setup_synth_kick_tilde();
void setup_synth_ks_tilde();
void setup_synth_marimba_tilde();
void setup_synth_metro_tilde();
void setup_synth_piano_tilde();
void setup_synth_risset_arp_tilde();
void setup_synth_risset_bell_tilde();
void setup_synth_risset_tone_tilde();
void setup_synth_russian_bell_tilde();
void setup_synth_snare_tilde();
void setup_synth_standard_bell_tilde();
void setup_synth_tube_bell();
void setup_synth_voice_fofc_tilde();
void setup_synth_voice_fofs_tilde();

void ceammc_synth_setup()
{
#ifdef STK_VERSION
    LIB_DBG << "STK version: " << STK_VERSION;
#endif

    // STK synths
    setup_synth_bthree();
    setup_synth_fgrain();
    setup_synth_rhodey();
    setup_synth_shakers();
    setup_synth_sitar();
    setup_synth_wurley();
    setup_synth_tube_bell();

    // faust synth
    setup_synth_birds_tilde();
    setup_synth_church_bell_tilde();
    setup_synth_clap_tilde();
    setup_synth_dubdub_tilde();
    setup_synth_dx7_tilde();
    setup_synth_eguitar_tilde();
    setup_synth_english_bell_tilde();
    setup_synth_french_bell_tilde();
    setup_synth_german_bell_tilde();
    setup_synth_glass_harm_tilde();
    setup_synth_harpsichord_tilde();
    setup_synth_hat_tilde();
    setup_synth_kick2_tilde();
    setup_synth_kick_tilde();
    setup_synth_ks_tilde();
    setup_synth_marimba_tilde();
    setup_synth_metro_tilde();
    setup_synth_piano_tilde();
    setup_synth_risset_arp_tilde();
    setup_synth_risset_bell_tilde();
    setup_synth_risset_tone_tilde();
    setup_synth_russian_bell_tilde();
    setup_synth_snare_tilde();
    setup_synth_standard_bell_tilde();
    setup_synth_voice_fofc_tilde();
    setup_synth_voice_fofs_tilde();

    // Glitch
    setup_synth_glitch();
}

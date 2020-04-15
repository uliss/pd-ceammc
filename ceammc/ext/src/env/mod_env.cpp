#include "mod_env.h"
#include "env_concat.h"
#include "env_env.h"
#include "env_mix.h"
#include "env_to_array.h"
#include "env_to_vline.h"
#include "env_tscale.h"
#include "env_tshift.h"
#include "env_vscale.h"
#include "vline_to_env.h"

extern void setup_env_adsr_tilde();
extern void setup_env_ar_tilde();
extern void setup_env_asr_tilde();
extern void setup_env_follow_tilde();
extern void setup_env_smooth_tilde();

void ceammc_env_setup()
{
    setup_env_adsr_tilde();
    setup_env_ar_tilde();
    setup_env_asr_tilde();
    setup_env_concat();
    setup_env_follow_tilde();
    setup_env_mix();
    setup_env_smooth_tilde();
    setup_env_to_array();
    setup_env_to_vline();
    setup_env_tscale();
    setup_env_tshift();
    setup_env_vscale();
    setup_envelope_env();
    setup_vline_to_env();
}

#include "mod_base.h"

#include "base_click.h"
#include "canvas_current.h"
#include "canvas_top.h"
#include "function.h"
#include "function_call.h"
#include "gain.h"
#include "matrix.h"
#include "metro_pattern.h"
#include "metro_seq.h"
#include "mix.h"
#include "obj_info.h"
#include "patch_args.h"
#include "radio.h"
#include "xfade2_tilde.h"
#include "xfade_tilde.h"

extern "C" void expand_env_setup();
extern "C" void is_any_setup();
extern "C" void is_bang_setup();
extern "C" void is_even_setup();
extern "C" void is_file_setup();
extern "C" void is_float_setup();
extern "C" void is_list_setup();
extern "C" void is_odd_setup();
extern "C" void is_pointer_setup();
extern "C" void is_symbol_setup();
extern "C" void msg_setup();
extern "C" void replace_setup();
extern "C" void setup_prop0x2eget();
extern "C" void setup_prop0x2eset();
extern "C" void setup_reject0x2eif();
extern "C" void setup_snd0x2efile();
extern "C" void setup_test0x2edata();
extern "C" void setup_test0x2eexpect();
extern "C" void setup_prop0x2eget_tilde();

void setup_is_data();
void setup_load_msg();

void ceammc_base_setup()
{
    click_tilde_setup();
    expand_env_setup();
    function_call_setup();
    function_setup();
    is_any_setup();
    is_bang_setup();
    is_even_setup();
    is_file_setup();
    is_float_setup();
    is_list_setup();
    is_odd_setup();
    is_pointer_setup();
    is_symbol_setup();
    msg_setup();
    replace_setup();
    setup_is_data();
    setup_load_msg();
    setup_metro_pattern();
    setup_metro_seq();
    setup_obj_info();
    setup_prop0x2eget();
    setup_prop0x2eset();

#ifdef WITH_SND_FILE
    setup_snd0x2efile();
#endif

    setup_test0x2edata();
    setup_test0x2eexpect();

    setup_canvas_current();
    setup_canvas_top();
    setup_gain_tilde();
    setup_base_mix();
    setup_base_matrix();
    setup_patch_args();
    setup_base_radio();
    setup_base_xfade_tilde();
    setup_base_xfade2_tilde();

    setup_prop0x2eget_tilde();
}

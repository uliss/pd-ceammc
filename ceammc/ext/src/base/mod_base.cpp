#include "mod_base.h"

#include "base_click.h"
#include "base_dac.h"
#include "base_log.h"
#include "base_spring.h"
#include "canvas_current.h"
#include "canvas_top.h"
#include "ceammc_search.h"
#include "function.h"
#include "function_call.h"
#include "gain.h"
#include "matrix.h"
#include "metro_pattern.h"
#include "metro_random.h"
#include "metro_seq.h"
#include "mix.h"
#include "msg_after.h"
#include "msg_sched.h"
#include "obj_props.h"
#include "patch_args.h"
#include "patch_deps.h"
#include "patch_props.h"
#include "patch_tree.h"
#include "prop.h"
#include "prop_declare.h"
#include "prop_random.h"
#include "radio.h"
#include "xfade2_tilde.h"
#include "xfade_tilde.h"

extern "C" void is_any_setup();
extern "C" void is_bang_setup();
extern "C" void is_even_setup();
extern "C" void is_float_setup();
extern "C" void is_list_setup();
extern "C" void is_odd_setup();
extern "C" void is_pointer_setup();
extern "C" void is_prop_setup();
extern "C" void is_symbol_setup();

extern "C" void setup_test0x2edata();
extern "C" void setup_test0x2eexpect();

void setup_base_bitmap();
void setup_base_canvas_active();
void setup_base_canvas_dir();
void setup_base_canvas_name();
void setup_base_canvas_path();
void setup_base_clone();
void setup_base_convolve_tilde();
void setup_base_expand_env();
void setup_base_gate_tilde();
void setup_base_merge_tilde();
void setup_base_msg();
void setup_base_msg_pack();
void setup_base_msg_unpack();
void setup_base_replace();
void setup_base_split_tilde();
void setup_base_sync();
void setup_base_z_tilde();
void setup_chrono_datetime();
void setup_is_data();
void setup_is_file();
void setup_load_expr();
void setup_metro_choice();
void setup_metro_wchoice();
void setup_msg_onclose();
void setup_msg_onload();
void setup_nsig_tilde();
void setup_prop_get();
void setup_prop_join();
void setup_prop_route();
void setup_prop_route_tilde();
void setup_prop_set();
void setup_snd_file();
void setup_snd_play_tilde();

void ceammc_base_setup()
{
    click_tilde_setup();
    is_any_setup();
    is_bang_setup();
    is_even_setup();
    is_float_setup();
    is_list_setup();
    is_odd_setup();
    is_pointer_setup();
    is_prop_setup();
    is_symbol_setup();

    setup_base_bitmap();
    setup_base_canvas_active();
    setup_base_canvas_dir();
    setup_base_canvas_name();
    setup_base_canvas_path();
    setup_base_clone();
    setup_base_convolve_tilde();
    setup_base_dac();
    setup_base_expand_env();
    setup_base_function();
    setup_base_gate_tilde();
    setup_base_log();
    setup_base_matrix();
    setup_base_merge_tilde();
    setup_base_mix();
    setup_base_msg();
    setup_base_msg_pack();
    setup_base_msg_unpack();
    setup_base_prop();
    setup_base_radio();
    setup_base_replace();
    setup_base_split_tilde();
    setup_base_spring();
    setup_base_sync();
    setup_base_xfade2_tilde();
    setup_base_xfade_tilde();
    setup_base_z_tilde();
    setup_canvas_current();
    setup_canvas_top();
    setup_ceammc_search();
    setup_chrono_datetime();
    setup_function_call();
    setup_gain_tilde();
    setup_is_data();
    setup_is_file();
    setup_load_expr();
    setup_msg_onload();
    setup_metro_choice();
    setup_metro_pattern();
    setup_metro_random();
    setup_metro_seq();
    setup_metro_wchoice();
    setup_msg_after();
    setup_msg_onclose();
    setup_msg_sched();
    setup_nsig_tilde();
    setup_obj_props();
    setup_patch_args();
    setup_patch_deps();
    setup_patch_props();
    setup_patch_tree();
    setup_prop_declare();
    setup_prop_get();
    setup_prop_route_tilde();
    setup_prop_join();
    setup_prop_random();
    setup_prop_set();
    setup_prop_route();
    setup_snd_file();
    setup_snd_play_tilde();

    setup_test0x2edata();
    setup_test0x2eexpect();
}

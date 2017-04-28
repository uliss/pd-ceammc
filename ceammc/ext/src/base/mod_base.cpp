#include "mod_base.h"

extern "C" void expand_env_setup();
extern "C" void setup_flow0x2esync();
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
extern "C" void setup_pass0x2echanged();
extern "C" void setup_pass0x2eif();
extern "C" void setup_pass0x2ethis();
extern "C" void setup_prop0x2eget();
extern "C" void setup_prop0x2eset();
extern "C" void setup_reject0x2eif();
extern "C" void setup_reject0x2ethis();
extern "C" void setup_snd0x2efile();
extern "C" void setup_test0x2edata();
extern "C" void setup_test0x2eexpect();

void ceammc_base_setup()
{
    setup_flow0x2esync();

    expand_env_setup();
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
    setup_pass0x2echanged();
    setup_pass0x2eif();
    setup_pass0x2ethis();
    setup_prop0x2eget();
    setup_prop0x2eset();
    setup_reject0x2eif();
    setup_reject0x2ethis();

#ifdef WITH_SND_FILE
    setup_snd0x2efile();
#endif

    setup_test0x2edata();
    setup_test0x2eexpect();
}

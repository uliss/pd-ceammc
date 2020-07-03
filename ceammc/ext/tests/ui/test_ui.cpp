#include "test_ui.h"
#include "mod_ui.h"

void test_ui_main_init()
{
    static bool init_done = false;

    if (!init_done) {
        epd_init();

        ceammc_ui_setup();

        LogExternalOutput::setup();
        ListenerExternal::setup();
        init_done = true;
    }
}

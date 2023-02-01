/*****************************************************************************
 * Copyright 2017 Serge Poltavsky. All rights reserved.
 *
 * This file may be distributed under the terms of GNU Public License version
 * 3 (GPL v3) as defined by the Free Software Foundation (FSF). A copy of the
 * license should have been included with this file, or the project in which
 * this file belongs to. You may also find the details of GPL v3 at:
 * http://www.gnu.org/licenses/gpl-3.0.txt
 *
 * If you have any questions regarding the use of this file, feel free to
 * contact the author of this file, or the owner of the project in which
 * this file belongs to.
 *****************************************************************************/
#define CATCH_CONFIG_MAIN

#ifdef __WIN32
#include <Winsock2.h>
#endif

#include "catch.hpp"

#include <chrono>
#include <future>
#include <thread>

#include "ceammc_preset.h"
#include "ceammc_datastorage.h"
#include "test_base.h"
#include "test_external.h"

extern "C" {
#include "g_canvas.h"
#include "s_stuff.h"

void sys_bail(int n);
int m_mainloop();
int m_batchmain(void);
void sys_exit();
void sys_stopgui();
void sched_reopenmeplease(void);
}

extern "C" void pd_init();

#ifdef __WIN32
static bool initWinSock()
{
    WSADATA wsaData;
    int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) {
        printf("WSAStartup failed: %d\n", iResult);
        return false;
    }

    return true;
}
#endif

namespace test {
void pdPrintToStdError(bool value)
{
    sys_printtostderr = value;
}

void pdSetPrintFunction(pdPrintFunction fn)
{
    sys_verbose = 1;
    STUFF->st_printhook = fn; // update for Pd-0.53
    sys_printhook = fn;
}

void pdRunMainLoopMs(int ms)
{
#ifdef __WIN32
    static bool ws = initWinSock();
#endif

#if defined(__linux__) || defined(__FreeBSD__)
    sys_hipriority = 0;
#endif

    sched_reopenmeplease();

    auto f = std::async(std::launch::async, [&]() {
        std::this_thread::sleep_for(std::chrono::milliseconds(ms));
        sys_exit();
        return 1;
    });

    //    sys_stopgui();
    setTestSampleRate(44100);
    m_mainloop();

    f.wait();
}

struct PdInitListener : Catch::TestEventListenerBase {
    using TestEventListenerBase::TestEventListenerBase;

    void testRunStarting(Catch::TestRunInfo const& testRunInfo) override
    {
        if (!pd_canvasmaker) {
            std::cerr << "[test] pd_init()" << std::endl;
            pd_init();
        }

        LogExternalOutput::setup();
        std::cerr << "[test] LogExternalOutput::setup()" << std::endl;

        ListenerExternal::setup();
        std::cerr << "[test] ListenerExternal::setup()" << std::endl;

        test::pdPrintToStdError();
    }

    void testRunEnded(Catch::TestRunStats const& testRunStats) override
    {
    }

    void testCaseStarting(Catch::TestCaseInfo const& testInfo) override
    {
        auto top = PureData::instance().createTopCanvas("test_canvas");

        auto cnv = canvas_getcurrent();
        if (!cnv)
            canvas_setcurrent(top->pd_canvas());
    }

    void testCaseEnded(Catch::TestCaseStats const& testCaseStats) override
    {
        t_canvas* cur = canvas_getcurrent();
        std::vector<t_canvas*> cnvl;

        auto glist = pd_getcanvaslist();
        for (auto c = glist; c != nullptr; c = c->gl_next) {
            if (c->gl_name->s_name[0] == '_')
                continue;

            cnvl.push_back(c);
        }

        if (cur)
            canvas_unsetcurrent(cur);

        for (auto c : cnvl)
            pd_free(&c->gl_obj.te_g.g_pd);

        PresetStorage::instance().clearAll();
    }
};

CATCH_REGISTER_LISTENER(PdInitListener)

}

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
#include "catch.hpp"

#include <chrono>
#include <future>
#include <thread>

#include "test_base.h"

extern "C" {
#include "s_stuff.h"

int m_mainloop();
int m_batchmain(void);
void sys_exit();
void sched_reopenmeplease(void);
int sys_nogui;
}

namespace test {
void pdPrintToStdError(bool value)
{
    sys_printtostderr = value;
}

void pdSetPrintFunction(pdPrintFunction fn)
{
    sys_printhook = fn;
}

void pdRunMainLoopMs(int ms)
{
    sched_reopenmeplease();

    auto f = std::async(std::launch::async, [&]() {
        std::this_thread::sleep_for(std::chrono::milliseconds(ms));
        sys_exit();
        return 1;
    });

    sys_nogui = 1;
    setTestSampleRate(44100);
    m_mainloop();

    f.wait();
}
}

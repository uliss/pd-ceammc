/*****************************************************************************
 * Copyright 2018 Serge Poltavsky. All rights reserved.
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
#ifndef TEST_COMMON_H
#define TEST_COMMON_H

#include "ceammc_message.h"

#include "test_approx.h"
#include "test_datatypes.h"
#include "test_macro.h"
#include "test_wrappers.h"

#include "catch.hpp"

using namespace ceammc;

namespace test {
// pd error/debug print
void pdPrintToStdError(bool value = true);

typedef void(pdPrintFunction)(const char*);
void pdSetPrintFunction(pdPrintFunction fn);

void pdRunMainLoopMs(int ms);
}

typedef std::vector<Message> MessageList;

#endif // TEST_COMMON_H

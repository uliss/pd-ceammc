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
#ifndef TEST_EXTERNAL_H
#define TEST_EXTERNAL_H

#include "ceammc_message.h"
#include "ceammc_pd.h"

using namespace ceammc;
using namespace ceammc::pd;

struct BANG__ {
    BANG__() {}
};

extern const BANG__ BANG;

class ExternalOutput : public External {
public:
    ExternalOutput();
    Message msg();
    void reset();

public:
    static void setup();
};

class ListenerExternal : public External {
public:
    ListenerExternal(const char* s);
    ListenerExternal(t_symbol* s);
    Message msg();
    t_symbol* addr();
    void reset();

public:
    static void setup();
    static bool isBinded(const char* s);
};

#endif // TEST_EXTERNAL_H

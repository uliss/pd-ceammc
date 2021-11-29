/*****************************************************************************
 * Copyright 2019 Serge Poltavsky. All rights reserved.
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
#ifndef SYSTEM_CURSOR_H
#define SYSTEM_CURSOR_H

#include "ceammc_clock.h"
#include "ceammc_object.h"

using namespace ceammc;

class SystemCursor : public BaseObject {
    ClockLambdaFunction unbind_;
    BoolProperty* relative_;
    BoolProperty* normalize_;
    BoolProperty* clip_;
    bool is_polling_;
    t_canvas* topcanvas_;

public:
    SystemCursor(const PdArgs& args);
    ~SystemCursor();

    void onBang() override;
    void onFloat(t_float f) override;

    void m_button(t_symbol* s, const AtomListView& lv);
    void m_motion(t_symbol* s, const AtomListView& lv);
    void m_wheel(t_symbol* s, const AtomListView& lv);

private:
    void checkPolling();
    void startPolling();
    void stopPolling();

private:
    static int instances_polling_;
};

void setup_system_cursor();

#endif // SYSTEM_CURSOR_H

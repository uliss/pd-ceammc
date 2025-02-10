/*****************************************************************************
 * Copyright 2025 Serge Poltavski. All rights reserved.
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
#ifndef HW_GPIO_H
#define HW_GPIO_H

#include "ceammc_object.h"
#include "ceammc_poll_dispatcher.h"
#include "hw_rust.hpp"
using namespace ceammc;

using HwGpioBase = DispatchedObject<BaseObject>;

class HwGpio : public HwGpioBase {
    ceammc_hw_gpio* gpio_ { nullptr };

public:
    HwGpio(const PdArgs& args);
    ~HwGpio();

    bool notify(int code) final;
    void m_set(t_symbol* s, const AtomListView& lv);
    void m_toggle(t_symbol* s, const AtomListView& lv);

private:
    static void on_error(void* data, const char* msg);
};

void setup_hw_gpio();

#endif // HW_GPIO_H

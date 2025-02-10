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
#include "hw_gpio.h"
#include "args/argcheck.h"
#include "ceammc_factory.h"

HwGpio::HwGpio(const PdArgs& args)
    : HwGpioBase(args)
{
    gpio_ = ceammc_hw_gpio_new(
        { this, on_error },
        { size_t(subscriberId()), [](size_t id) { Dispatcher::instance().send(NotifyMessage { id, 0 }); } } //
    );
}

HwGpio::~HwGpio()
{
    ceammc_hw_gpio_free(gpio_);
}

bool HwGpio::notify(int code)
{
    ceammc_hw_gpio_process_events(gpio_);
    return true;
}

void HwGpio::m_read(t_symbol *s, const AtomListView &lv)
{
    if (!args::check_args("PIN:b", lv, this))
        return;

    ceammc_hw_gpio_read_pin(gpio_, lv.intAt(0, 0));
}

void HwGpio::m_write(t_symbol* s, const AtomListView& lv)
{
    if (!args::check_args("PIN:b VALUE:B", lv, this))
        return;

    ceammc_hw_gpio_write_pin(gpio_, lv.intAt(0, 0), lv.intAt(1, 1));
}

void HwGpio::m_toggle(t_symbol* s, const AtomListView& lv)
{
    if (!args::check_args("PIN:b", lv, this))
        return;

    ceammc_hw_gpio_toggle_pin(gpio_, lv.intAt(0, 0));
}

void HwGpio::on_error(void* data, const char* msg)
{
    LIB_ERR << msg;
}

void setup_hw_gpio()
{
    ObjectFactory<HwGpio> obj("hw.gpio");
    obj.addMethod("read", &HwGpio::m_read);
    obj.addMethod("write", &HwGpio::m_write);
    obj.addMethod("toggle", &HwGpio::m_toggle);
}

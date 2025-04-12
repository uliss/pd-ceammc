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
#ifndef HW_RPI_GPIO_H
#define HW_RPI_GPIO_H

#include "ceammc_object.h"
#include "hw_rust.hpp"
#include "rust_dispatched_object.h"
using namespace ceammc;

class HwRpiGpio : public RustDispatchedObject<BaseObject> {
    ceammc_hw_gpio* gpio_ { nullptr };

public:
    explicit HwRpiGpio(const PdArgs& args);
    ~HwRpiGpio();

    bool notify(int code) final;

    void m_clear_pwm(t_symbol* s, const AtomListView& lv);
    void m_impulse(t_symbol* s, const AtomListView& lv);
    void m_input(t_symbol* s, const AtomListView& lv);
    void m_output(t_symbol* s, const AtomListView& lv);
    void m_poll(t_symbol* s, const AtomListView& lv);
    void m_pull_down(t_symbol* s, const AtomListView& lv);
    void m_pull_up(t_symbol* s, const AtomListView& lv);
    void m_read(t_symbol* s, const AtomListView& lv);
    void m_reset(t_symbol* s, const AtomListView& lv);
    void m_soft_pwm(t_symbol* s, const AtomListView& lv);
    void m_soft_pwm_freq(t_symbol* s, const AtomListView& lv);
    void m_toggle(t_symbol* s, const AtomListView& lv);
    void m_used_pins(t_symbol* s, const AtomListView& lv);
    void m_write(t_symbol* s, const AtomListView& lv);

private:
    static void on_pin_value(void* data, std::uint8_t pin, bool value);
};

void setup_hw_rpi_gpio();

#endif // HW_RPI_GPIO_H

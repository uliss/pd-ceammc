#ifndef HW_RPI_PWM_H
#define HW_RPI_PWM_H

#include "ceammc_object.h"
#include "hw_rpi_device.h"
#include "hw_rust.hpp"
using namespace ceammc;

class HwRpiPwm : public HwRpiDevice<ceammc_hw_rpi_pwm> {
    IntProperty* chan_ { nullptr };

public:
    explicit HwRpiPwm(const PdArgs& args);

    bool notify(int code) final;

    void m_duty(t_symbol* s, const AtomListView& lv);
    void m_enable(t_symbol* s, const AtomListView& lv);
    void m_freq(t_symbol* s, const AtomListView& lv);
    void m_period(t_symbol* s, const AtomListView& lv);
    void m_polarity(t_symbol* s, const AtomListView& lv);
    void m_pwm(t_symbol* s, const AtomListView& lv);
    void m_width(t_symbol* s, const AtomListView& lv);

    Device createDevice() final;
};

void setup_hw_rpi_pwm();

#endif // HW_RPI_PWM_H

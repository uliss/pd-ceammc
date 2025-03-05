#ifndef HW_RPI_PWM_H
#define HW_RPI_PWM_H

#include "ceammc_object.h"
#include "ceammc_poll_dispatcher.h"
#include "hw_rust.hpp"
using namespace ceammc;

class HwRpiPwm : public DispatchedObject<BaseObject> {
    ceammc_hw_rpi_pwm* pwm_ { nullptr };
    IntProperty* chan_ { nullptr };

public:
    HwRpiPwm(const PdArgs& args);
    ~HwRpiPwm();

    void initDone() final;
    bool notify(int code) final;

    void m_duty(t_symbol* s, const AtomListView& lv);
    void m_enable(t_symbol* s, const AtomListView& lv);
    void m_freq(t_symbol* s, const AtomListView& lv);
    void m_period(t_symbol* s, const AtomListView& lv);
    void m_polarity(t_symbol* s, const AtomListView& lv);
    void m_pwm(t_symbol* s, const AtomListView& lv);
    void m_width(t_symbol* s, const AtomListView& lv);
};

void setup_hw_rpi_pwm();

#endif // HW_RPI_PWM_H

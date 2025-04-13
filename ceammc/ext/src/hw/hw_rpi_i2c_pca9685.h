#ifndef HW_RPI_I2C_PCA9685_H
#define HW_RPI_I2C_PCA9685_H

#include "ceammc_object.h"
#include "hw_rust.hpp"
#include "rust_dispatched_object.h"
using namespace ceammc;

class HwI2cPca8695 : public RustDispatchedObject<BaseObject> {
    ceammc_hw_pca9685* pwm_ { nullptr };
    IntProperty* chan_ { nullptr };
    I2cBusProperty* i2c_bus_ { nullptr };
    I2cAddrProperty* i2c_addr_ { nullptr };

public:
    explicit HwI2cPca8695(const PdArgs& args);
    ~HwI2cPca8695();

    void initDone() final;
    bool notify(int code) final;

    void m_const(t_symbol* s, const AtomListView& lv);
    void m_disable_prog_addr(t_symbol* s, const AtomListView& lv);
    void m_duty(t_symbol* s, const AtomListView& lv);
    void m_enable(t_symbol* s, const AtomListView& lv);
    void m_enable_restart_and_disable(t_symbol* s, const AtomListView& lv);
    void m_freq(t_symbol* s, const AtomListView& lv);
    void m_period(t_symbol* s, const AtomListView& lv);
    void m_polarity(t_symbol* s, const AtomListView& lv);
    void m_pulse_width(t_symbol* s, const AtomListView& lv);
    void m_restart(t_symbol* s, const AtomListView& lv);
    void m_set_raw(t_symbol* s, const AtomListView& lv);
    void m_use_prog_addr(t_symbol* s, const AtomListView& lv);
};

void setup_hw_rpi_i2c_pca9685();

#endif // HW_RPI_I2C_PCA9685_H

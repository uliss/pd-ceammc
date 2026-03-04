#ifndef HW_RPI_I2C_PCA9685_H
#define HW_RPI_I2C_PCA9685_H

#include "ceammc_object.h"
#include "hw_rust.hpp"
#include "rust_dispatched_object.h"
using namespace ceammc;

using Pca9685Ptr = std::unique_ptr<ceammc_hw_pca9685, void (*)(ceammc_hw_pca9685*)>;

class HwI2cPca8695 : public RustDispatchedObject<BaseObject> {
    Pca9685Ptr pwm_;
    IntProperty* chan_ { nullptr };
    I2cBusProperty* i2c_bus_ { nullptr };
    I2cAddrProperty* i2c_addr_ { nullptr };
    BoolProperty* connect_ { nullptr };

public:
    explicit HwI2cPca8695(const PdArgs& args);

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

private:
    bool check_connected(bool print_err);
    void i2c_connect(bool state);
};

void setup_hw_rpi_i2c_pca9685();

#endif // HW_RPI_I2C_PCA9685_H

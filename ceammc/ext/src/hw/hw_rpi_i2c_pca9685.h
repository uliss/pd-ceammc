#ifndef HW_RPI_I2C_PCA9685_H
#define HW_RPI_I2C_PCA9685_H

#include "ceammc_object.h"
#include "ceammc_poll_dispatcher.h"
#include "hw_rust.hpp"
using namespace ceammc;

class HwI2cPca8695 : public DispatchedObject<BaseObject> {
    ceammc_hw_pca9685* pwm_ { nullptr };
    IntProperty* chan_ { nullptr };

public:
    explicit HwI2cPca8695(const PdArgs& args);
    ~HwI2cPca8695();

    void initDone() final;
    bool notify(int code) final;

    void m_duty(t_symbol* s, const AtomListView& lv);
    void m_enable(t_symbol* s, const AtomListView& lv);
    void m_freq(t_symbol* s, const AtomListView& lv);
    void m_period(t_symbol* s, const AtomListView& lv);
    void m_polarity(t_symbol* s, const AtomListView& lv);
    void m_set_raw(t_symbol* s, const AtomListView& lv);
};

void setup_hw_rpi_i2c_pca9685();

#endif // HW_RPI_I2C_PCA9685_H

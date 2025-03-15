#ifndef HW_RPI_SENSOR_VL53L0X_H
#define HW_RPI_SENSOR_VL53L0X_H

#include "ceammc_object.h"
#include "rust_dispatched_object.h"
using namespace ceammc;

class HwRpiSensorVl53l0x : public RustDispatchedObject<BaseObject> {
    ceammc_hw_sensor_vl53l0x* vc_ { nullptr };
    IntProperty* i2c_bus_ { nullptr };
    IntProperty* i2c_addr_ { nullptr };

public:
    explicit HwRpiSensorVl53l0x(const PdArgs& args);
    ~HwRpiSensorVl53l0x();

    void initDone() final;
    bool notify(int code) final;

    void onBang() final;

    void m_poll(t_symbol* s, const AtomListView& lv);
    void m_address(t_symbol* s, const AtomListView& lv);
};

void setup_hw_rpi_sensor_vl53l0x();

#endif // HW_RPI_SENSOR_VL53L0X_H

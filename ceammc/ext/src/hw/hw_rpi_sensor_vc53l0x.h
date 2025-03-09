#ifndef HW_RPI_SENSOR_VC53L0X_H
#define HW_RPI_SENSOR_VC53L0X_H

#include "ceammc_object.h"
#include "ceammc_poll_dispatcher.h"
#include "rust_dispatched_object.h"
using namespace ceammc;

class HwRpiSensorVc53l0x : public RustDispatchedObject<BaseObject> {
    ceammc_hw_sensor_vl53l0x* vc_ { nullptr };

public:
    explicit HwRpiSensorVc53l0x(const PdArgs& args);
    ~HwRpiSensorVc53l0x();

    void initDone() final;
    bool notify(int code) final;

    void onBang() final;

    void m_poll(t_symbol* s, const AtomListView& lv);
};

void setup_hw_rpi_sensor_vc53l0x();

#endif // HW_RPI_SENSOR_VC53L0X_H

#ifndef HW_RPI_SENSOR_IR_H
#define HW_RPI_SENSOR_IR_H

#include "ceammc_object.h"
#include "hw_rust.hpp"
#include "rust_dispatched_object.h"
using namespace ceammc;

class HwRpiSensorIR : public RustDispatchedObject<BaseObject> {
    ceammc_hw_infrared* ir_ { nullptr };
    IntProperty* pin_ { nullptr };

public:
    explicit HwRpiSensorIR(const PdArgs& args);
    ~HwRpiSensorIR();

    void initDone() final;
    bool notify(int code) final;

//    void onBang() final;
//    void m_poll(t_symbol* s, const AtomListView& lv);
};

void setup_hw_rpi_sensor_ir();

#endif // HW_RPI_SENSOR_IR_H

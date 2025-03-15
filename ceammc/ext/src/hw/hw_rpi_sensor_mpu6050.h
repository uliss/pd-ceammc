#ifndef HW_RPI_SENSOR_MPU6050_H
#define HW_RPI_SENSOR_MPU6050_H

#include "ceammc_object.h"
#include "ceammc_property_enum.h"
#include "hw_rust.hpp"
#include "rust_dispatched_object.h"
using namespace ceammc;

class HwRpiSensorMpu6050 : public RustDispatchedObject<BaseObject> {
    ceammc_hw_mpu6050* mpu_ { nullptr };
    IntProperty* i2c_bus_ { nullptr };
    //    SymbolEnumProperty* mode_ { nullptr };
    //    SymbolFloatEnumProperty* fsr_ { nullptr };
    //    FloatProperty* poll_time_ { nullptr };
    //    // normalization
    //    BoolProperty* normalize_ { nullptr };
    //    FloatProperty* out_min_ { nullptr };
    //    FloatProperty* out_max_ { nullptr };
    //    FloatProperty* in_min_ { nullptr };
    //    FloatProperty* in_max_ { nullptr };
    //    //
    //    t_symbol* sym_channel_ { nullptr };

public:
    explicit HwRpiSensorMpu6050(const PdArgs& args);
    ~HwRpiSensorMpu6050();

    void initDone() final;
    bool notify(int code) final;

    void m_calibrate(t_symbol* s, const AtomListView& lv);
    void m_poll(t_symbol* s, const AtomListView& lv);
    //    void m_measure(t_symbol* s, const AtomListView& lv);

private:
    void outputData(float yaw, float pitch, float roll);
    //     t_float normalizeValue(int16_t value) const;
    //     void outputValue(std::uint8_t ch, std::int16_t value);
};

void setup_hw_rpi_sensor_mpu6050();

#endif // HW_RPI_SENSOR_MPU6050_H

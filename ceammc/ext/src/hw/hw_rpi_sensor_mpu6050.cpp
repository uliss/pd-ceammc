#include "hw_rpi_sensor_mpu6050.h"
#include "args/argcheck.h"
#include "ceammc_containers.h"
#include "ceammc_factory.h"

HwRpiSensorMpu6050::HwRpiSensorMpu6050(const PdArgs& args)
    : RustDispatchedObject<BaseObject>(args)
    , mpu_(nullptr, ceammc_hw_mpu6050_free)
{
    createOutlet();

    i2c_bus_ = addI2cBusProperty();
    i2c_addr_ = addI2cAddrProperty();

    connect_ = new BoolProperty("@connect", false);
    connect_->setSuccessFn([this](Property*) {
        i2c_connect(connect_->value());
    });
    addProperty(connect_);
}

bool HwRpiSensorMpu6050::notify(int code)
{
    return ceammc_hw_mpu6050_process_reply(mpu_.get());
}

void HwRpiSensorMpu6050::m_calibrate(t_symbol* s, const AtomListView& lv)
{
    if (!check_connected(true))
        return;

    ceammc_hw_mpu6050_calibrate(mpu_.get());
}

void HwRpiSensorMpu6050::m_poll(t_symbol* s, const AtomListView& lv)
{
    static const args::ArgChecker chk("STATE:B");
    if (!chk.check(lv, this, s))
        return chk.usage(this, s);

    if (!check_connected(true))
        return;

    ceammc_hw_mpu6050_poll(mpu_.get(), lv.boolAt(0, false));
}

void HwRpiSensorMpu6050::output_data(float yaw, float pitch, float roll)
{
    AtomArray<3> ypr { yaw, pitch, roll };
    anyTo(0, gensym("ypr"), ypr.view());
}

bool HwRpiSensorMpu6050::check_connected(bool print_err)
{
    if (!mpu_ && print_err) {
        OBJ_ERR << "device is not connected";
    }

    return mpu_.get();
}

void HwRpiSensorMpu6050::i2c_connect(bool state)
{
    if (state) {
        std::int8_t bus = 0;
        if (!i2c_bus_->getBus(bus)) {
            OBJ_ERR << "i2c bus is no set";
            return;
        }

        mpu_.reset(ceammc_hw_mpu6050_new(bus,
            i2c_addr_->value(),
            on_notify(),
            on_message(),
            { this,
                [](void* user, float yaw, float pitch, float roll) {
                    auto obj = static_cast<HwRpiSensorMpu6050*>(user);
                    if (!obj)
                        return;

                    obj->output_data(yaw, pitch, roll);
                },
                [](void* user, float temp) {
                    auto obj = static_cast<HwRpiSensorMpu6050*>(user);
                    if (!obj)
                        return;

                    obj->anyTo(0, gensym("temp"), temp);
                } }));

        if (!mpu_)
            OBJ_ERR << "can't connect to device";
    } else {
        mpu_.reset();
    }
}

void setup_hw_rpi_sensor_mpu6050()
{
    ObjectFactory<HwRpiSensorMpu6050> obj("hw.rpi.sensor.mpu6050");

    CEAMMC_OBJECT_ADD_METHOD(obj, HwRpiSensorMpu6050, poll)
    CEAMMC_OBJECT_ADD_METHOD(obj, HwRpiSensorMpu6050, calibrate)
}

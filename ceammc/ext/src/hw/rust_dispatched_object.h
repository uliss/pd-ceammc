#ifndef RUST_DISPATCHED_OBJECT_H
#define RUST_DISPATCHED_OBJECT_H

#include "ceammc_object.h"
#include "ceammc_poll_dispatcher.h"
#include "ceammc_property_enum.h"
#include "hw_rust.hpp"

#define CEAMMC_OBJECT_ADD_METHOD(obj, cls, method) obj.addMethod(#method, &cls::m_##method);

namespace ceammc {

class I2cBusProperty : public EnumProperty<Atom> {
public:
    I2cBusProperty(const char* name)
        : EnumProperty<Atom>(name, { Atom(gensym("none")), 1, 2, 3, 4, 5, 6, gensym("default") })
    {
    }

    bool isNone() const { return value() == "none"; }
    bool isValid() const { return !isNone(); }

    bool getBus(std::int8_t& bus) const
    {
        if (isNone()) {
            return false;
        } else if (value() == "default") {
            bus = ceammc_HW_I2C_DEFAULT_BUS;
            return true;
        } else {
            bus = value().asT<t_int>();
            return true;
        }
    }
};

template <class T>
class RustDispatchedObject : public DispatchedObject<T> {
public:
    RustDispatchedObject(const PdArgs& args)
        : DispatchedObject<T>(args)
    {
    }

protected:
    ceammc_hw_notify_cb on_notify() const
    {
        return {
            this->subscriberId(), [](size_t id) { Dispatcher::instance().send({ id, 0 }); }
        };
    }

    ceammc_hw_msg_cb on_message()
    {
        return { static_cast<void*>(this),
            [](void* user, ceammc_hw_msg_level level, const char* msg) {
                auto obj = static_cast<typeof(this)>(user);
                switch (level) {
                case ceammc_hw_msg_level::Debug:
                    Debug(obj) << msg;
                    break;
                case ceammc_hw_msg_level::Info:
                    Post(obj) << msg;
                    break;
                default:
                    Error(obj) << msg;
                    break;
                }
            } };
    }

    I2cBusProperty* addI2cBusProperty()
    {
        auto prop = new I2cBusProperty("@i2c_bus");
        prop->setInitOnly();
        this->addProperty(prop);
        return prop;
    }

    IntProperty* addI2cAddrProperty()
    {
        auto prop = new IntProperty("@i2c_addr", ceammc_HW_I2C_DEFAULT_ADDR);
        prop->setInitOnly();
        prop->checkClosedRange(ceammc_HW_I2C_MIN_ADDR, ceammc_HW_I2C_MAX_ADDR);
        this->addProperty(prop);
        return prop;
    }

    IntProperty* addPwmChanProperty()
    {
        auto prop = new IntProperty("@pwm_ch", ceammc_HW_RPI_PWM_NONE_CHAN);
        prop->setInitOnly();
        prop->checkClosedRange(ceammc_HW_RPI_PWM_MIN_CHAN, ceammc_HW_RPI_PWM_MAX_CHAN);
        this->addProperty(prop);
        return prop;
    }

    IntProperty* addSpiBusProperty()
    {
        auto prop = new IntProperty("@spi_bus", static_cast<int>(ceammc_hw_spi_bus::SPI0));
        prop->setInitOnly();
        prop->checkClosedRange(static_cast<int>(ceammc_hw_spi_bus::SPI0), static_cast<int>(ceammc_hw_spi_bus::SPI6));
        this->addProperty(prop);
        return prop;
    }

    IntProperty* addSpiCsProperty()
    {
        auto prop = new IntProperty("@spi_cs", static_cast<int>(ceammc_hw_spi_cs::CS0));
        prop->setInitOnly();
        prop->checkClosedRange(static_cast<int>(ceammc_hw_spi_cs::CS0), static_cast<int>(ceammc_hw_spi_cs::CS3));
        this->addProperty(prop);
        return prop;
    }

    IntProperty* addSpiFreqProperty()
    {
        auto prop = new IntProperty("@spi_freq", 1000000);
        prop->setUnitsHz();
        prop->setInitOnly();
        prop->checkClosedRange(100000, 10000000);
        this->addProperty(prop);
        return prop;
    }

    IntProperty* addGpioPinProperty(const char* name)
    {
        auto prop = new IntProperty(name, -1);
        prop->setInitOnly();
        prop->checkClosedRange(-1, 255);
        this->addProperty(prop);
        return prop;
    }
};

}

#endif // RUST_DISPATCHED_OBJECT_H

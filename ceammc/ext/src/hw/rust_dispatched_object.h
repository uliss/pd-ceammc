#ifndef RUST_DISPATCHED_OBJECT_H
#define RUST_DISPATCHED_OBJECT_H

#include "ceammc_object.h"
#include "ceammc_poll_dispatcher.h"
#include "ceammc_property_enum.h"
#include "hw_rust.hpp"

#define CEAMMC_OBJECT_ADD_METHOD(obj, cls, method) obj.addMethod(#method, &cls::m_##method);

namespace ceammc {

class SpiBusProperty : public IntProperty {
public:
    explicit SpiBusProperty(const char* name, ceammc_hw_spi_bus def = ceammc_hw_spi_bus::NONE)
        : IntProperty(name, static_cast<int>(def))
    {
        checkClosedRange(static_cast<int>(ceammc_hw_spi_bus::NONE), static_cast<int>(ceammc_hw_spi_bus::SPI6));
    }

    bool isNone(bool print_err) const
    {
        auto res = (value() == static_cast<int>(ceammc_hw_spi_bus::NONE));

        if (!res && print_err)
            LogPdObject(owner(), LOG_ERROR) << "spi bus is not set";

        return res;
    }

    ceammc_hw_spi_bus bus() const
    {
        return static_cast<ceammc_hw_spi_bus>(value());
    }
};

class SpiCsPinProperty : public IntProperty {
public:
    explicit SpiCsPinProperty(const char* name, ceammc_hw_spi_cs def = ceammc_hw_spi_cs::CS0)
        : IntProperty(name, static_cast<int>(def))
    {
        checkClosedRange(static_cast<int>(ceammc_hw_spi_cs::CS0), static_cast<int>(ceammc_hw_spi_cs::CS3));
    }

    ceammc_hw_spi_cs pin() const
    {
        return static_cast<ceammc_hw_spi_cs>(value());
    }
};

class I2cBusProperty : public EnumProperty<Atom> {
public:
    explicit I2cBusProperty(const char* name)
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

class I2cAddrProperty : public IntProperty {
public:
    explicit I2cAddrProperty(const char* name, t_int def = ceammc_HW_I2C_NO_ADDR)
        : IntProperty(name, def)
    {
        checkClosedRange(ceammc_HW_I2C_MIN_ADDR, ceammc_HW_I2C_MAX_ADDR);
    }

    bool setList(const AtomListView& lv) final
    {
        if (!emptyCheck(lv))
            return false;

        if (lv.isInteger())
            return setInt(lv.asInt());
        else if (lv == gensym("default"))
            return setInt(ceammc_HW_I2C_DEFAULT_ADDR);
        else if (lv == gensym("alt"))
            return setInt(ceammc_HW_I2C_ALT_ADDR);
        else if (lv == gensym("auto"))
            return setInt(ceammc_HW_I2C_AUTO_ADDR);
        else if (lv == gensym("none"))
            return setInt(ceammc_HW_I2C_NO_ADDR);
        else {
            LIB_ERR << '[' << name()->s_name << "] invalid property value: " << lv;
            return false;
        }
    }
};

class GpioPinProperty : public IntProperty {
public:
    GpioPinProperty(const char* name)
        : IntProperty(name, ceammc_HW_GPIO_DEF_PIN)
    {
        setInitOnly();
        checkClosedRange(ceammc_HW_GPIO_MIN_PIN, ceammc_HW_GPIO_MAX_PIN);
    }

    bool isNone() const { return value() == ceammc_HW_GPIO_PIN_NONE; }
    bool isValid() const { return !isNone(); }

    bool checkPin(BaseObject* obj) const
    {
        if (isNone()) {
            Error(obj) << "GPIO pin is not specified: " << name()->s_name;
            return false;
        } else
            return true;
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

    I2cAddrProperty* addI2cAddrProperty()
    {
        auto prop = new I2cAddrProperty("@i2c_addr", ceammc_HW_I2C_DEFAULT_ADDR);
        prop->setInitOnly();
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

    SpiBusProperty* addSpiBusProperty()
    {
        auto prop = new SpiBusProperty("@spi_bus", ceammc_hw_spi_bus::NONE);
        prop->setInitOnly();
        this->addProperty(prop);
        return prop;
    }

    SpiCsPinProperty* addSpiCsProperty()
    {
        auto prop = new SpiCsPinProperty("@spi_cs", ceammc_hw_spi_cs::CS0);
        prop->setInitOnly();
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

    GpioPinProperty* addGpioPinProperty(const char* name)
    {
        auto prop = new GpioPinProperty(name);
        this->addProperty(prop);
        return prop;
    }
};

}

#endif // RUST_DISPATCHED_OBJECT_H

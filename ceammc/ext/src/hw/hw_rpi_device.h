/*****************************************************************************
 * Copyright 2026 Serge Poltavski. All rights reserved.
 *
 * This file may be distributed under the terms of GNU Public License version
 * 3 (GPL v3) as defined by the Free Software Foundation (FSF). A copy of the
 * license should have been included with this file, or the project in which
 * this file belongs to. You may also find the details of GPL v3 at:
 * http://www.gnu.org/licenses/gpl-3.0.txt
 *
 * If you have any questions regarding the use of this file, feel free to
 * contact the author of this file, or the owner of the project in which
 * this file belongs to.
 *****************************************************************************/
#ifndef HW_RPI_DEVICE_H
#define HW_RPI_DEVICE_H

#include "ceammc_rs_msg_object.h"
#include "hw_rpi_device_props.h"

#define CEAMMC_OBJECT_ADD_METHOD(obj, cls, method) obj.addMethod(#method, &cls::m_##method);

namespace ceammc {

template <typename HardwareHandle>
class HwRpiDevice : public RustMessageObject<BaseObject> {
public:
    using FreeDeviceFn = void (*)(HardwareHandle*);
    using Device = std::unique_ptr<HardwareHandle, FreeDeviceFn>;

    static const auto CONNECT_PRIORITY = 128;
    static const auto DEVICE_CONNECTED_PRIORITY = 160;

public:
    HwRpiDevice(FreeDeviceFn fn, const PdArgs& args)
        : RustMessageObject<BaseObject>(args)
        , hw_(nullptr, fn)
        , free_fn_(fn)
    {

        connect_ = new BoolProperty("@connect", false);
        connect_->info().setPriority(CONNECT_PRIORITY);
        connect_->setSuccessFn([this](Property*) {
            connect(connect_->value());
        });
        addProperty(connect_);
    }

    virtual bool check_connected(bool print_error, t_symbol* s)
    {
        if (!hw_ && print_error) {
            if (s && s != &s_)
                METHOD_ERR(s) << "device is not connected";
            else
                OBJ_ERR << "device is not connected";
        }

        return hw_.get();
    }

    virtual void connect(bool state)
    {
        if (state) {
            if (hw_) {
                OBJ_DBG << "already connected";
                return;
            }

            hw_ = std::move(createDevice());

            if (!hw_)
                OBJ_ERR << "can't connect to the device";
        } else {
            hw_.reset();
        }
    }

    virtual Device createDevice() = 0;

    HardwareHandle* device() { return hw_.get(); }
    const HardwareHandle* device() const { return hw_.get(); }
    FreeDeviceFn freeDeviceFn() const { return free_fn_; }

    void freeDevice()
    {
        hw_.reset();
    }

    Device nullDevice() const
    {
        return Device(nullptr, free_fn_);
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

private:
    BoolProperty* connect_ { nullptr };
    Device hw_;
    FreeDeviceFn free_fn_ { nullptr };
};

} // namespace ceammc

#endif // HW_RPI_DEVICE_H

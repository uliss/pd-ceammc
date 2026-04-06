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
#ifndef HW_RPI_DEVICE_PROPS_H
#define HW_RPI_DEVICE_PROPS_H

#include "ceammc_property.h"
#include "ceammc_property_enum.h"
#include "hw_rust.hpp"

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
        auto is_none = (value() == static_cast<int>(ceammc_hw_spi_bus::NONE));

        if (is_none && print_err)
            LogPdObject(owner(), LOG_ERROR) << "spi bus is not set";

        return is_none;
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
    explicit GpioPinProperty(const char* name)
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
} // namespace ceammc

#endif // HW_RPI_DEVICE_PROPS_H

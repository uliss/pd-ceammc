/*****************************************************************************
 * Copyright 2018 Serge Poltavsky. All rights reserved.
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
#ifndef CEAMMC_PROPERTY_INFO_H
#define CEAMMC_PROPERTY_INFO_H

#include <boost/variant.hpp>
#include <string>
#include <vector>

namespace ceammc {

typedef boost::variant<bool, int, float, std::string> PropertySingleValue;
typedef std::vector<PropertySingleValue> PropertyListValue;
typedef boost::variant<PropertySingleValue, PropertyListValue> PropertyValue;

enum class PropertyInfoType {
    BOOLEAN = 0,
    INTEGER,
    FLOAT,
    SYMBOL,
    LIST
};

enum class PropertyInfoView {
    SLIDER = 0,
    KNOB,
    NUMBOX,
    SPINBOX,
    TOGGLE,
    MENU,
    ENTRY,
    COLOR
};

enum class PropertyInfoUnits {
    MSEC = 0,
    SEC,
    SAMPLES,
    DB
};

class PropertyInfo {
    std::string name_;
    PropertyInfoType type_;
    PropertyInfoView view_;
    float min_, max_;
    float step_;
    PropertyValue default_;
    PropertyListValue enum_;
    std::string units_;

public:
    PropertyInfo(const std::string& name, PropertyInfoType type);

    const std::string& name() const { return name_; }
    PropertyInfoType type() const { return type_; }
    PropertyInfoView view() const { return view_; }
    float min() const { return min_; }
    float max() const { return max_; }
    float step() const { return step_; }

    bool hasMinLimit() const;
    bool hasMaxLimit() const;
    bool hasEnumLimit() const;
    bool hasStep() const;

    const PropertyValue& defaultValue() const { return default_; }
    const PropertyListValue& enumValues() const { return enum_; }

    void setDefault(bool v);
    void setDefault(int v);
    void setDefault(float v);
    void setDefault(const std::string& s);
    void setDefault(const PropertyListValue& lst);

    bool setMin(float v);
    bool setMax(float v);
    void setRange(float min, float max);

    bool setStep(float step);
    bool setView(PropertyInfoView v);

    void addEnum(int v);
    void addEnum(const std::string& s);

    const std::string units() const { return units_; }
    void setUnits(const std::string& s);

    bool defaultBool(bool def = false) const;
    int defaultInt(int def = 0) const;
    float defaultFloat(float def = 0) const;
    std::string defaultString(const std::string& s = "") const;
};
}

#endif // CEAMMC_PROPERTY_INFO_H

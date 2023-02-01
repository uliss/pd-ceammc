/*****************************************************************************
 * Copyright 2020 Serge Poltavsky. All rights reserved.
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
#ifndef CEAMMC_PROPERTY_ENUM_H
#define CEAMMC_PROPERTY_ENUM_H

#include "ceammc_property.h"

#include <initializer_list>

namespace ceammc {

template <typename T>
class EnumProperty : public Property {
public:
    EnumProperty(const std::string& name, T def, PropValueAccess access = PropValueAccess::READWRITE)
        : Property(PropertyInfo(name, PropertyInfo::toType<T>()), access)
        , current_idx_(0)
    {
        if (!info().setConstraints(PropValueConstraints::ENUM)) {
            LogPdObject(owner(), LogLevel::LOG_ERROR).stream() << errorPrefix() << "can't set constraints";
            return;
        }

        if (!info().addEnum(def)) {
            LogPdObject(owner(), LogLevel::LOG_ERROR).stream() << errorPrefix() << "can't append value to enum: " << def;
            return;
        }

        info().setDefault(def);
        setView(PropValueView::MENU);
    }

    EnumProperty(const std::string& name, std::initializer_list<T> values, PropValueAccess access = PropValueAccess::READWRITE)
        : Property(PropertyInfo(name, PropertyInfo::toType<T>()), access)
        , current_idx_(0)
    {
        if (!info().setConstraints(PropValueConstraints::ENUM)) {
            LogPdObject(owner(), LogLevel::LOG_ERROR).stream() << errorPrefix() << "can't set constraints";
            return;
        }

        setView(PropValueView::MENU);

        if (values.size() == 0) {
            LogPdObject(owner(), LogLevel::LOG_ERROR).stream() << errorPrefix() << "empty initializer";
            return;
        }

        // set default info
        info().setDefault(*values.begin());

        if (!info().addEnums(values)) {
            LogPdObject(owner(), LogLevel::LOG_ERROR).stream() << errorPrefix() << "can't append values to enum";
            return;
        }
    }

    T defaultValue() const
    {
        T def;
        if (!getDefault(def)) {
            LogPdObject(owner(), LogLevel::LOG_ERROR).stream() << errorPrefix() << "can't get default value";
            throw std::runtime_error("error");
        } else {
            return def;
        }
    }

    AtomList get() const override
    {
        return listFrom(value());
    }

    bool setList(const AtomListView& lv) override
    {
        if (!emptyCheck(lv))
            return false;

        if (lv.size() > 1) {
            LogPdObject(owner(), LogLevel::LOG_ERROR).stream() << errorPrefix() << "single value expected, got: " << lv;
            return false;
        }

        const Atom& a = lv[0];
        if (!a.template isA<T>()) {
            LogPdObject(owner(), LogLevel::LOG_ERROR).stream() << errorPrefix() << "invalid value type: " << a;
            return false;
        }

        return setValue(a.template asT<T>());
    }

    size_t numEnums() const { return info().enumCount(); }

    T value() const
    {
        return info().enumValues()[current_idx_].template asT<T>();
    }

    bool setValue(T v)
    {
        long idx = enumIndex(v);
        if (idx < 0) {
            LogPdObject(owner(), LogLevel::LOG_ERROR).stream() << errorPrefix() << "invalid property value: " << v
                                                               << ", valid values are: " << info().enumValues();
            return false;
        }

        current_idx_ = static_cast<decltype(current_idx_)>(idx);
        return true;
    }

    size_t index() const { return current_idx_; }

    bool setIndex(size_t idx)
    {
        if (idx >= info().enumCount()) {
            LogPdObject(owner(), LogLevel::LOG_ERROR).stream()
                << errorPrefix() << "invalid enum index, expecting value in [0.." << numEnums() << "] range, got: " << idx;
            return false;
        } else {
            current_idx_ = idx;
            return true;
        }
    }

    long enumIndex(T v) const
    {
        return info().enumValues().findPos(atomFrom(v));
    }

    bool appendEnum(T v)
    {
        if (enumIndex(v) < 0) {
            if (!info().addEnum(v)) {
                LogPdObject(owner(), LogLevel::LOG_ERROR).stream() << errorPrefix() << "can't add enum value: " << v;
                return false;
            }

            return true;
        } else {
            LogPdObject(owner(), LogLevel::LOG_ERROR).stream() << errorPrefix() << "value already exists in enum: " << v;
            return false;
        }
    }

public:
    using value_type = T;

private:
    size_t current_idx_;
};

/**
 * @brief int enum property
 */
using IntEnumProperty = EnumProperty<int>;

/**
 * @brief symbol property only with allowed values
 */
class SymbolEnumProperty : public EnumProperty<t_symbol*> {
public:
    SymbolEnumProperty(const std::string& name, t_symbol* def, PropValueAccess access = PropValueAccess::READWRITE);
    SymbolEnumProperty(const std::string& name, std::initializer_list<t_symbol*> values, PropValueAccess access = PropValueAccess::READWRITE);
    SymbolEnumProperty(const std::string& name, std::initializer_list<const char*> values, PropValueAccess access = PropValueAccess::READWRITE);

    bool setList(const AtomListView& lv) override;
};

/**
 * alias to symbol: @wrap -> @mode wrap
 */
using SymbolEnumAlias = AliasProperty<SymbolEnumProperty>;

}

#endif // CEAMMC_PROPERTY_ENUM_H

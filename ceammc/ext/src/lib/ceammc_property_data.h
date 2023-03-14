/*****************************************************************************
 * Copyright 2023 Serge Poltavski. All rights reserved.
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
#ifndef CEAMMC_PROPERTY_DATA_H
#define CEAMMC_PROPERTY_DATA_H

#include "ceammc_data.h"
#include "ceammc_property.h"

namespace ceammc {
template <class T>
class DataPropertyT : public Property {
    DataAtom<T> v_;

public:
    DataPropertyT(const std::string& name, const T& def, PropValueAccess access = PropValueAccess::READWRITE)
        : Property(PropertyInfo(name, PropValueType::ATOM), access)
        , v_(def)
    {
    }

    bool setList(const AtomListView& lv) override
    {
        if (!lv.isA<T>())
            return false;

        return setValue(*lv[0].asDataT<T>());
    }

    AtomList get() const override { return v_; }
    bool getAtom(Atom& a) const override
    {
        a = v_;
        return true;
    }

    bool setBool(bool b) override { return false; }
    bool setFloat(t_float f) override { return false; }
    bool setInt(int i) override { return false; }
    bool setSymbol(t_symbol* s) override { return false; }
    bool setAtom(const Atom& a) override
    {
        if (!a.isDataType(T::dataType))
            return false;
        else {
            v_ = std::move(DataAtom<T>(a));
            return true;
        }
    }

    inline T& value() { return *v_; }
    inline const T& value() const { return *v_; }
    bool setValue(const T& v)
    {
        v_ = DataAtom<T>(v);
        return true;
    }

    DataAtom<T> asDataAtom() const { return DataAtom<T>(v_); }
};
}

#endif // CEAMMC_PROPERTY_DATA_H

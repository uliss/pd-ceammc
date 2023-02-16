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
#ifndef RHYTHM_LSYSTEM_H
#define RHYTHM_LSYSTEM_H

#include "ceammc_data.h"
#include "ceammc_object.h"
using namespace ceammc;

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
    //    bool setAtom(const Atom& a) override { return false; }

    inline T& value() { return *v_; }
    inline const T& value() const { return *v_; }
    bool setValue(const T& v)
    {
        v_ = DataAtom<T>(v);
        return true;
    }
    //    T defaultValue() const;
};

using DictProperty = DataPropertyT<DataTypeDict>;

class RhythmLsystem : public BaseObject {
    ListProperty* init_;
    ListProperty* value_;
    DictProperty* rule_;
    IntProperty* max_;

public:
    RhythmLsystem(const PdArgs& args);

    void initDone() override;
    void onBang() override;

    void m_reset(t_symbol* s, const AtomListView& lv);
};

void setup_rhythm_lsystem();

#endif // RHYTHM_LSYSTEM_H

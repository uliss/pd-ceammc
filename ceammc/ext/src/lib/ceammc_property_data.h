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
#include "ceammc_format.h"
#include "ceammc_property.h"
#include "ceammc_string.h"

namespace ceammc {
template <class T>
class DataPropertyT : public Property {
    DataAtom<T> v_;
    bool parse_messages_;

public:
    DataPropertyT(
        const std::string& name,
        const T& def,
        bool parse = true,
        PropValueAccess access = PropValueAccess::READWRITE)
        : Property(PropertyInfo(name, PropValueType::ATOM), access)
        , v_(def)
        , parse_messages_(parse)
    {
        info().setDefault(v_);
    }

    AtomList get() const final { return v_; }
    bool setList(const AtomListView& lv) final
    {
        if (!lv.isAtom())
            return false;
        else
            return setAtom(lv.front());
    }

    bool getAtom(Atom& a) const final
    {
        a = v_;
        return true;
    }

    bool setAtom(const Atom& a) final
    {
        if (!a.isA<T>()) {
            if (parse_messages_) {
                auto res = parseDataList(AtomListView(a));
                if (!res)
                    return false;

                if (!res.result().isA<T>()) {
                    LIB_ERR << '[' << name()->s_name << "] only "
                            << string::atom_type_name(v_).c_str() << " datatype is expected, got: "
                            << string::atom_type_name(a).c_str();
                    return false;
                }

                v_ = DataAtom<T>(*res.result().asD<T>());
                return true;
            } else
                return false;
        } else {
            v_ = DataAtom<T>(a);
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

template <class T>
class DataPropertyListT : public Property {
    std::vector<T> v_;
    bool parse_messages_;

public:
    DataPropertyListT(
        const std::string& name,
        const std::vector<T>& def,
        bool parse = true,
        PropValueAccess access = PropValueAccess::READWRITE)
        : Property(PropertyInfo(name, PropValueType::LIST), access)
        , v_(def)
        , parse_messages_(parse)
    {
    }

    AtomList get() const final
    {
        AtomList res;
        res.reserve(v_.size());
        for (auto& x : v_)
            res.push_back(DataAtom<T>(x));

        return res;
    }

    bool getList(AtomList& lst) const final
    {
        lst.clear();
        lst.reserve(v_.size());
        for (auto& x : v_)
            lst.push_back(DataAtom<T>(x));

        return true;
    }

    bool setList(const AtomListView& lv) final
    {
        if (lv.empty()) {
            v_.clear();
            return true;
        }

        if (!lv.allOf([](const Atom& a) { return a.isA<T>(); })) {
            if (!parse_messages_) {
                LIB_ERR << '[' << name()->s_name << "] only "
                        << T().typeName().c_str() << " datatypes are expected";
                return false;
            }

            std::vector<T> data;
            data.reserve(lv.size());

            for (auto& a : lv) {
                auto res = parseDataList(AtomListView(a));
                if (!res)
                    return false;

                if (!res.result().isA<T>()) {
                    LIB_ERR << '[' << name()->s_name << "] only "
                            << T().typeName().c_str() << " datatypes are expected, got: "
                            << (res.result().isData()
                                       ? string::atom_type_name(res.result()[0]).c_str()
                                       : string::atom_type_name(a).c_str());
                    return false;
                }

                data.push_back(*res.result().asD<T>());
            }

            v_ = std::move(data);
            return true;
        } else {
            v_.clear();
            v_.reserve(lv.size());

            for (auto& a : lv)
                v_.push_back(*a.asD<T>());

            return true;
        }
    }

    std::vector<T>& value() { return v_; }
    const std::vector<T>& value() const { return v_; }
    void setValue(const std::vector<T>& v) { v_ = v; }

    size_t size() const { return v_.size(); }
};
}

#endif // CEAMMC_PROPERTY_DATA_H

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
#include "fmt/core.h"

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
    }

    bool setList(const AtomListView& lv) override
    {
        if (!lv.isA<T>()) {
            if (parse_messages_) {
                auto res = parseDataList(lv);
                if (!res || !res.result().isA<T>()) {
                    LIB_ERR << fmt::format("[{}] only {} datatype is expected, got: {}",
                        name()->s_name,
                        v_->typeName().c_str(),
                        to_string(lv));
                    return false;
                }

                return setValue(*res.result()[0].asDataT<T>());
            } else
                return false;
        }

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

    bool setList(const AtomListView& lv) override
    {
        if (lv.empty()) {
            v_.clear();
            return true;
        }

        if (!lv.allOf([](const Atom& a) { return a.isA<T>(); })) {
            if (!parse_messages_) {
                LIB_ERR << fmt::format("[{}] only {} datatypes are expected",
                    name()->s_name,
                    T().typeName().c_str());
                return false;
            }

            std::vector<T> data;
            data.reserve(lv.size());

            for (auto& a : lv) {
                auto res = parseDataList(AtomListView(a));
                if (!res || !res.result().isA<T>()) {
                    LIB_ERR << fmt::format("[{}] only {} datatypes are expected, got: {}",
                        name()->s_name,
                        T().typeName().c_str(),
                        to_string(a));
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

    AtomList get() const final
    {
        AtomList res;
        res.reserve(v_.size());
        for (auto& x : v_)
            res.push_back(DataAtom<T>(x));

        return res;
    }

    std::vector<T>& value() { return v_; }
    const std::vector<T>& value() const { return v_; }
    void setValue(const std::vector<T>& v) { v_ = v; }

    size_t size() const { return v_.size(); }
};
}

#endif // CEAMMC_PROPERTY_DATA_H

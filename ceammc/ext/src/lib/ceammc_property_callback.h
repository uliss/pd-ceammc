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
#ifndef CEAMMC_PROPERTY_CALLBACK_H
#define CEAMMC_PROPERTY_CALLBACK_H

#include "ceammc_property.h"

namespace ceammc {

class CallbackProperty : public Property {
public:
    enum class Type : uint8_t {
        BOOL = 0,
        FLOAT,
        INT,
        SYMBOL,
        ATOM,
        LIST
    };

    struct FnGetterUnion {
        FnGetterUnion(const PropertyBoolGetter& f);
        FnGetterUnion(const PropertyFloatGetter& f);
        FnGetterUnion(const PropertyIntGetter& f);
        FnGetterUnion(const PropertySymbolGetter& f);
        FnGetterUnion(const PropertyAtomGetter& f);
        FnGetterUnion(const PropertyListGetter& f);
        ~FnGetterUnion();

        union {
            PropertyBoolGetter fn_bool;
            PropertyFloatGetter fn_float;
            PropertyIntGetter fn_int;
            PropertySymbolGetter fn_symbol;
            PropertyAtomGetter fn_atom;
            PropertyListGetter fn_list;
        };

        const Type type;
    };

    struct FnSetterUnion {
        FnSetterUnion(const PropertyBoolSetter& f);
        FnSetterUnion(const PropertyFloatSetter& f);
        FnSetterUnion(const PropertyIntSetter& f);
        FnSetterUnion(const PropertySymbolSetter& f);
        FnSetterUnion(const PropertyAtomSetter& f);
        FnSetterUnion(const PropertyListSetter& f);
        ~FnSetterUnion();

        const Type type;
        union {
            PropertyBoolSetter fn_bool;
            PropertyFloatSetter fn_float;
            PropertyIntSetter fn_int;
            PropertySymbolSetter fn_symbol;
            PropertyAtomSetter fn_atom;
            PropertyListSetter fn_list;
        };
    };

    template <class T>
    struct tag {
        using type = T;
    };

    template <typename T>
    struct function_traits
        : public function_traits<decltype(&T::operator())> {
    };
    // For generic types, directly use the result of the signature of its 'operator()'

    template <typename ClassType, typename ReturnType, typename... Args>
    struct function_traits<ReturnType (ClassType::*)(Args...) const>
    // we specialize for pointers to member function
    {
        typedef ReturnType result_type;
    };

private:
    FnGetterUnion getter_;
    FnSetterUnion setter_;
    std::string cb_err_msg_;

    CallbackProperty(const std::string& name, PropertyBoolGetter getter, PropertyBoolSetter setter, tag<bool>);
    CallbackProperty(const std::string& name, PropertyFloatGetter getter, PropertyFloatSetter setter, tag<t_float>);
    CallbackProperty(const std::string& name, PropertyIntGetter getter, PropertyIntSetter setter, tag<int>);
    CallbackProperty(const std::string& name, PropertySymbolGetter getter, PropertySymbolSetter setter, tag<t_symbol*>);
    CallbackProperty(const std::string& name, PropertyAtomGetter getter, PropertyAtomSetter setter, tag<Atom>);
    CallbackProperty(const std::string& name, PropertyListGetter getter, PropertyListSetter setter, tag<AtomList>);

public:
    template <class G, class S>
    CallbackProperty(const std::string& name, G getter, S setter = nullptr, const std::string& setterErrMsg = std::string())
        : CallbackProperty(name, getter, setter,
            tag<typename function_traits<decltype(getter)>::result_type>())
    {
        cb_err_msg_ = setterErrMsg;
    }

    AtomList get() const override;

    bool getBool(bool&) const final;
    bool getFloat(t_float& f) const final;
    bool getInt(int& i) const final;
    bool getSymbol(t_symbol*& s) const final;
    bool getAtom(Atom& a) const final;
    bool getList(AtomList& l) const final;

    bool setBool(bool b) final;
    bool setFloat(t_float f) final;
    bool setInt(int i) final;
    bool setSymbol(t_symbol* s) final;
    bool setAtom(const Atom& a) final;
    bool setList(const AtomListView& lv) final;

    bool checkPositive() final;
    bool checkNegative() final;
    bool checkNonNegative() final;

    bool checkMin(t_float v);
    bool checkMinEq(t_float v);
    bool checkMax(t_float v);
    bool checkMaxEq(t_float v);
    bool checkClosedRange(t_float a, t_float b);
    bool checkOpenedRange(t_float a, t_float b);
    bool checkNonZero();

    void updateDefault() override;

private:
    enum CallbackType {
        GETTER = 0,
        SETTER
    };

    bool hasBoolCb(CallbackType t) const;
    bool hasFloatCb(CallbackType t) const;
    bool hasIntCb(CallbackType t) const;
    bool hasSymbolCb(CallbackType t) const;
    bool hasAtomCb(CallbackType t) const;
    bool hasListCb(CallbackType t) const;
};

}

#endif // CEAMMC_PROPERTY_CALLBACK_H

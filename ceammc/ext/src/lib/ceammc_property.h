/*****************************************************************************
 * Copyright 2016 Serge Poltavsky. All rights reserved.
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
#ifndef CEAMMC_PROPERTY_H
#define CEAMMC_PROPERTY_H

#include "ceammc_atomlist.h"
#include "ceammc_log.h"
#include "ceammc_property_info.h"

#include <functional>
#include <initializer_list>
#include <iosfwd>
#include <iterator>
#include <memory>
#include <string>
#include <tuple>
#include <type_traits>
#include <vector>

namespace ceammc {

t_symbol* SYM_DUMP();
t_symbol* SYM_PROPS_ALL();
t_symbol* SYM_PROPS_ALL_Q();

using PropertyBoolGetter = std::function<bool()>;
using PropertyBoolSetter = std::function<bool(bool)>;
using PropertyFloatGetter = std::function<t_float()>;
using PropertyFloatSetter = std::function<bool(t_float)>;
using PropertyIntGetter = std::function<int()>;
using PropertyIntSetter = std::function<bool(int)>;
using PropertySymbolGetter = std::function<t_symbol*()>;
using PropertySymbolSetter = std::function<bool(t_symbol*)>;
using PropertyAtomGetter = std::function<Atom()>;
using PropertyAtomSetter = std::function<bool(const Atom&)>;
using PropertyListGetter = std::function<AtomList()>;
using PropertyListSetter = std::function<bool(const AtomListView&)>;

/**
 * @brief Base class for all properties
 */
class Property {
public:
    using PropFloatCheckFn = std::function<bool(t_float)>;
    using PropIntCheckFn = std::function<bool(int)>;
    using PropSymbolCheckFn = std::function<bool(t_symbol*)>;
    using PropAtomCheckFn = std::function<bool(const Atom&)>;
    using PropListCheckFn = std::function<bool(const AtomListView&)>;
    using PropSuccessFn = std::function<void(Property*)>;

    using CheckFnTuple = std::tuple<
        PropFloatCheckFn,
        PropIntCheckFn,
        PropSymbolCheckFn,
        PropAtomCheckFn,
        PropListCheckFn>;

    using CheckFnPtr = std::unique_ptr<CheckFnTuple>;
    using SuccessFnPtr = std::unique_ptr<PropSuccessFn>;

public:
    Property(const PropertyInfo& info, PropValueAccess access = PropValueAccess::READWRITE);
    virtual ~Property();

    /// pure virtual
    virtual AtomList get() const = 0;
    virtual bool setList(const AtomListView& lv) = 0;
    /// virtual
    virtual bool set(const AtomListView& lv);
    virtual bool setInit(const AtomListView& lv);

    inline t_symbol* name() const { return info_.name(); }

    inline const PropertyInfo& info() const { return info_; }

    inline PropValueAccess access() const { return info_.access(); }
    inline PropValueType type() const { return info_.type(); }
    inline PropValueUnits units() const { return info_.units(); }
    inline PropValueView view() const { return info_.view(); }
    inline PropValueVis visibility() const { return info_.visibility(); }
    inline int8_t argIndex() const { return info_.argIndex(); }
    inline bool hasArgIndex() const { return info_.hasArgIndex(); }

    inline void setReadOnly() { info_.setAccess(PropValueAccess::READONLY); }
    inline void setInitOnly() { info_.setAccess(PropValueAccess::INITONLY); }
    inline void setPublic() { info_.setVisibility(PropValueVis::PUBLIC); }
    inline void setHidden() { info_.setVisibility(PropValueVis::HIDDEN); }
    inline void setInternal() { info_.setVisibility(PropValueVis::INTERNAL); }
    bool setArgIndex(int8_t idx);
    bool setArgIndexNext(Property* p);

    inline void setType(PropValueType t) { info_.setType(t); }
    inline void setUnits(PropValueUnits u) { info_.setUnits(u); }
    inline void setView(PropValueView v) { info_.setView(v); }
    inline void setVisibility(PropValueVis v) { info_.setVisibility(v); }

    bool getDefault(bool& b) const;
    bool getDefault(t_float& f) const;
    bool getDefault(int& i) const;
    bool getDefault(t_symbol*& s) const;
    bool getDefault(Atom& a) const;
    bool getDefault(AtomList& l) const;
    // some types of properties (callbacks for example)
    // can call virtual methods to get default value (and storing it info property info)
    // so to prevent them from calling such methods
    // when object is still constructing - we can update default info later with this call
    virtual void updateDefault();

    template <typename T>
    inline void setDefault(T v) { info_.setDefault(v); }

    // set to default value
    bool reset();

    inline bool isPublic() const { return info_.isPublic(); }
    inline bool isInternal() const { return info_.isInternal(); }
    inline bool isHidden() const { return info_.isHidden(); }
    inline bool isReadOnly() const { return info_.isReadOnly(); }
    inline bool isInitOnly() const { return info_.isInitOnly(); }
    inline bool isReadWrite() const { return info_.isReadWrite(); }
    inline bool isBool() const { return info_.isBool(); }
    inline bool isInt() const { return info_.isInt(); }
    inline bool isFloat() const { return info_.isFloat(); }
    inline bool isNumeric() const { return info_.isNumeric(); }
    inline bool isSymbol() const { return info_.isSymbol(); }
    inline bool isAtom() const { return info_.isVariant(); }
    inline bool isList() const { return info_.isList(); }

    bool setFloatCheckFn(PropFloatCheckFn fn, const std::string& err = std::string());
    bool setIntCheckFn(PropIntCheckFn fn, const std::string& err = std::string());
    bool setSymbolCheckFn(PropSymbolCheckFn fn, const std::string& err = std::string());
    bool setAtomCheckFn(PropAtomCheckFn fn, const std::string& err = std::string());
    bool setListCheckFn(PropListCheckFn fn, const std::string& err = std::string());
    bool setSuccessFn(PropSuccessFn fn);

    bool setFloatCheck(PropValueConstraints type, t_float a = 0, t_float b = 0);
    bool setIntCheck(PropValueConstraints type, int a = 0, int b = 0);
    bool setSymbolEnumCheck(std::initializer_list<t_symbol*> l);
    bool setSymbolEnumCheck(std::initializer_list<const char*> l);
    void setCheckErrorMsg(const std::string& str);

    bool callSuccessFn();

    virtual bool checkPositive();
    virtual bool checkNegative();
    virtual bool checkNonNegative();

    virtual bool getBool(bool&) const;
    virtual bool getFloat(t_float&) const;
    virtual bool getInt(int&) const;
    virtual bool getSymbol(t_symbol*&) const;
    virtual bool getAtom(Atom&) const;
    virtual bool getList(AtomList&) const;

    // generic get
    template <typename T>
    inline bool getT(T&) const;

    virtual bool setBool(bool);
    virtual bool setFloat(t_float);
    virtual bool setInt(int);
    virtual bool setSymbol(t_symbol*);
    virtual bool setAtom(const Atom&);

    // generic set
    inline bool setT(bool b) { return setBool(b); }
    inline bool setT(t_float f) { return setFloat(f); }
    inline bool setT(int i) { return setInt(i); }
    inline bool setT(t_symbol* s) { return setSymbol(s); }
    inline bool setT(const Atom& a) { return setAtom(a); }
    inline bool setT(const AtomListView& lv) { return setList(lv); }

    // freq used
    inline void setUnitsHz() { setUnits(PropValueUnits::HZ); }
    inline void setUnitsDb() { setUnits(PropValueUnits::DB); }
    inline void setUnitsMs() { setUnits(PropValueUnits::MSEC); }
    inline void setUnitsSec() { setUnits(PropValueUnits::SEC); }
    inline void setUnitsSamp() { setUnits(PropValueUnits::SAMP); }

    void setOwner(void* p) { owner_ = p; }
    const void* owner() const { return owner_; }

    PropertyInfo& infoT() { return info_; }

protected:
    PropertyInfo& info() { return info_; }
    bool checkAtom(const Atom& a) const;
    bool checkFloat(t_float v) const;
    bool checkInt(int v) const;
    bool checkList(const AtomListView& lv) const;
    bool checkSymbol(t_symbol* s) const;
    bool emptyCheck(const AtomListView& v) const;
    bool initCheck() const;
    bool writeCheck() const;
    std::string errorPrefix() const;

    template <typename V>
    inline bool checkValueT(const V& v);

    template <typename V>
    inline bool isA() const { return info_.isA<V>(); }

private:
    PropertyInfo info_;
    SuccessFnPtr ok_fn_ptr_;
    std::string err_msg_;
    void* owner_;

protected:
    CheckFnPtr check_fn_ptr_;
};

template <>
inline bool Property::checkValueT<bool>(const bool&) { return true; }
template <>
inline bool Property::checkValueT<t_float>(const t_float& v) { return checkFloat(v); }
template <>
inline bool Property::checkValueT<int>(const int& v) { return checkInt(v); }
template <>
inline bool Property::checkValueT<t_symbol*>(t_symbol* const& s) { return checkSymbol(s); }
template <>
inline bool Property::checkValueT<Atom>(const Atom& a) { return checkAtom(a); }
template <>
inline bool Property::checkValueT<AtomList>(const AtomList& l) { return checkList(l); }

template <>
inline bool Property::getT(bool& b) const { return getBool(b); }
template <>
inline bool Property::getT(t_float& f) const { return getFloat(f); }
template <>
inline bool Property::getT(int& i) const { return getInt(i); }
template <>
inline bool Property::getT(t_symbol*& s) const { return getSymbol(s); }
template <>
inline bool Property::getT(Atom& a) const { return getAtom(a); }
template <>
inline bool Property::getT(AtomList& l) const { return getList(l); }

/**
 * @brief Atom property: float or symbol
 */
class AtomProperty : public Property {
    Atom v_;

public:
    AtomProperty(const std::string& name, const Atom& def, PropValueAccess access = PropValueAccess::READWRITE);

    bool setList(const AtomListView& lv) override;
    AtomList get() const override;
    bool getAtom(Atom& a) const override;

    bool setBool(bool b) override;
    bool setFloat(t_float f) override;
    bool setInt(int i) override;
    bool setSymbol(t_symbol* s) override;
    bool setAtom(const Atom& a) override;

    inline Atom& value() { return v_; }
    inline const Atom& value() const { return v_; }
    bool setValue(const Atom& v);
    Atom defaultValue() const;

public:
    using value_type = Atom;
};

/**
 * @brief float property
 */
class FloatProperty : public Property {
    t_float v_;

public:
    FloatProperty(const std::string& name, t_float init = 0, PropValueAccess access = PropValueAccess::READWRITE);

    AtomList get() const override;
    bool setList(const AtomListView& lv) override;
    bool setFloat(t_float v) override;
    bool setInt(int v) override;
    bool getFloat(t_float& v) const override;

    inline t_float value() const { return v_; }
    bool setValue(t_float v);
    bool setValue(const Atom& a);
    t_float defaultValue() const;

    bool checkMin(t_float v) { return setFloatCheck(PropValueConstraints::GREATER_THEN, v); }
    bool checkMinEq(t_float v) { return setFloatCheck(PropValueConstraints::GREATER_EQUAL, v); }
    bool checkMax(t_float v) { return setFloatCheck(PropValueConstraints::LESS_THEN, v); }
    bool checkMaxEq(t_float v) { return setFloatCheck(PropValueConstraints::LESS_EQUAL, v); }
    bool checkClosedRange(t_float a, t_float b) { return setFloatCheck(PropValueConstraints::CLOSED_RANGE, a, b); }
    bool checkOpenedRange(t_float a, t_float b) { return setFloatCheck(PropValueConstraints::OPEN_RANGE, a, b); }
    bool checkNonZero() { return setFloatCheck(PropValueConstraints::NON_ZERO); }

    bool checkPositive() final { return checkMin(0); }
    bool checkNegative() final { return checkMax(0); }
    bool checkNonNegative() final { return checkMinEq(0); }

public:
    using value_type = t_float;
};

/**
 * @brief integer property
 */
class IntProperty : public Property {
    int v_;

public:
    IntProperty(const std::string& name, int init = 0, PropValueAccess access = PropValueAccess::READWRITE);

    AtomList get() const override;
    bool setList(const AtomListView& lv) override;
    bool setInt(int v) override;
    bool getInt(int& v) const override;

    inline int value() const { return v_; }
    bool setValue(int v);
    bool setValue(t_float f);
    bool setValue(const Atom& a);
    int defaultValue() const;

    bool checkMin(int v) { return setIntCheck(PropValueConstraints::GREATER_THEN, v); }
    bool checkMinEq(int v) { return setIntCheck(PropValueConstraints::GREATER_EQUAL, v); }
    bool checkMax(int v) { return setIntCheck(PropValueConstraints::LESS_THEN, v); }
    bool checkMaxEq(int v) { return setIntCheck(PropValueConstraints::LESS_EQUAL, v); }
    bool checkClosedRange(int a, int b) { return setIntCheck(PropValueConstraints::CLOSED_RANGE, a, b); }
    bool checkOpenedRange(int a, int b) { return setIntCheck(PropValueConstraints::OPEN_RANGE, a, b); }
    bool checkNonZero() { return setIntCheck(PropValueConstraints::NON_ZERO); }

    bool checkPositive() final { return checkMin(0); }
    bool checkNegative() final { return checkMax(0); }
    bool checkNonNegative() final { return checkMinEq(0); }

public:
    using value_type = int;
};

/**
 * @brief unsigned integer property
 */
class SizeTProperty : public Property {
    size_t v_;

public:
    SizeTProperty(const std::string& name, size_t init = 0, PropValueAccess access = PropValueAccess::READWRITE);

    AtomList get() const override;
    bool setList(const AtomListView& lv) override;
    bool getInt(int&) const override;

    inline size_t value() const { return v_; }
    bool setValue(size_t v);
    bool setValue(t_float f);
    bool setValue(const Atom& a);
    size_t defaultValue() const;

    bool checkPositive() final;
    bool checkNonNegative() final { return true; }

public:
    using value_type = size_t;
};

/**
 * @brief symbol property
 */
class SymbolProperty : public Property {
    mutable t_symbol* value_;

public:
    SymbolProperty(const std::string& name, t_symbol* init, PropValueAccess access = PropValueAccess::READWRITE);

    AtomList get() const override;
    bool setList(const AtomListView& lv) override;
    bool setSymbol(t_symbol* s) override;
    bool getSymbol(t_symbol*& s) const override;

    inline t_symbol* value() const { return value_; }
    bool setValue(t_symbol* s);
    bool setValue(const Atom& a);
    t_symbol* defaultValue() const;

    std::string str() const { return value_->s_name; }

public:
    using value_type = t_symbol*;
};

/**
 * @brief list property
 */
class ListProperty : public Property {
    AtomList lst_;
    PropAtomCheckFn filter_;
    AtomMapFunction map_;

public:
    ListProperty(const std::string& name, const AtomList& init = AtomList(), PropValueAccess access = PropValueAccess::READWRITE);

    AtomList get() const override;
    bool setList(const AtomListView& lv) override;
    bool getList(AtomList& l) const override;

    inline const AtomList& value() const { return lst_; }
    inline AtomList& value() { return lst_; }
    bool setValue(const AtomList& l);
    const AtomList& defaultValue() const;

    bool checkNonNegative() override;
    bool checkPositive() override;
    bool checkNegative() override;

    bool checkMinElementCount(size_t n);
    bool checkMaxElementCount(size_t n);
    bool checkRangeElementCount(size_t min, size_t max);

    /**
     * pass atoms from list for which fn returns true
     * the order is: filter -> map -> check
     */
    void setFilterAtomFn(PropAtomCheckFn pred);

    /**
     * maps each list value
     * the order is: filter -> map -> check
     **/
    void setMapAtomFn(AtomMapFunction fn);

    // easy to use filter function setup
    void acceptFloats();
    void acceptIntegers();
    void acceptSymbols();
    // ready map function
    void roundFloats();
    void truncateFloats();

public:
    using value_type = AtomList;
};

/**
 * @brief readonly property for getting several props at once.
 * @example property @pos could be defined for getting @x and @y properties
 * @note property is hidden: cab be set via messages, but not shown in UI
 */
class CombinedProperty : public Property {
    std::vector<Property*> props_;

public:
    CombinedProperty(const std::string& name, std::initializer_list<Property*> props);

    /**
     * always return true
     */
    bool setList(const AtomListView&) override;

    /**
     * @return concatenated list of combined properties
     */
    AtomList get() const override;

    bool getList(AtomList& l) const override;
};

/**
 * @brief boolean initonly property defined like [obj @flag]
 * if flag is specified - it equals to true, if not - to false
 */
class FlagProperty : public Property {
    bool v_;

public:
    FlagProperty(const std::string& name);

    AtomList get() const override;
    bool getBool(bool& b) const override;
    bool getInt(int& v) const override;
    bool setList(const AtomListView&) override;

    inline bool value() const { return v_; }

public:
    using value_type = bool;
};

/**
 * alias to property: @wrap -> @mode V
 */
template <typename T>
class AliasProperty : public Property {
public:
    using value_type = typename T::value_type;

private:
    T* ptr_;
    value_type val_;

public:
    AliasProperty(const std::string& name, T* prop, typename T::value_type v)
        : Property(PropertyInfo(name, prop->type()), prop->access())
        , ptr_(prop)
        , val_(v)
    {
        static_assert(std::is_base_of<Property, T>::value, "should be base of Property");

        setType(prop->type());
        setUnits(prop->units());
        setView(prop->view());
        setHidden();
    }

    value_type value() const { return val_; }

    bool setList(const AtomListView& lv) override
    {
        if (!lv.empty())
            LIB_ERR << "no arguments required for alias property: " << name();

        return ptr_->setValue(val_);
    }

    AtomList get() const override { return listFrom(bool(ptr_->value() == val_)); }
};

/**
 * @brief boolean property
 * @note @prop 1 and @prop true|false supported
 */
class BoolProperty : public Property {
    bool v_;

public:
    BoolProperty(const std::string& name, bool init, PropValueAccess access = PropValueAccess::READWRITE);

    AtomList get() const override;
    bool setList(const AtomListView& lv) override;
    bool setBool(bool b) override;
    bool getBool(bool& b) const override;

    bool value() const { return v_; }
    bool setValue(bool v);
    bool setValue(const Atom& a);
    bool defaultValue() const;

public:
    using value_type = bool;
};

/**
 * writes property value to pointer
 */
template <typename T>
class PointerProperty : public Property {
    T* vptr_;

public:
    PointerProperty(const std::string& name, T* value, PropValueAccess access = PropValueAccess::READONLY)
        : Property(PropertyInfo(name, PropertyInfo::toType<T>()), access)
        , vptr_(value)
    {
        info().setDefault(*value);
    }

    T value() const { return *vptr_; }

    bool setValue(const T& v)
    {
        if (!checkValueT(v))
            return false;

        *vptr_ = v;
        return true;
    }

    bool setList(const AtomListView& lv) override
    {
        if (!emptyCheck(lv))
            return false;

        if (!lv.isA<T>())
            return false;

        return setValue(atomlistToValue(lv, T()));
    }

    AtomList get() const override
    {
        return listFrom(*vptr_);
    }

    bool setBool(bool b) override { return setBoolT(b); }
    bool setFloat(t_float f) override { return setFloatT(f); }
    bool setInt(int i) override { return setIntT(i); }
    bool setSymbol(t_symbol* s) override { return setSymbolT(s); }
    bool setAtom(const Atom& a) override { return setAtomT(a); }

private:
    bool setBoolT(bool) { return false; }
    bool setFloatT(t_float) { return false; }
    bool setIntT(int) { return false; }
    bool setSymbolT(t_symbol*) { return false; }
    bool setAtomT(const Atom&) { return false; }

public:
    using value_type = T;
};

template <>
inline bool PointerProperty<bool>::setBoolT(bool b) { return setValue(b); }
template <>
inline bool PointerProperty<t_float>::setFloatT(t_float f) { return setValue(f); }
template <>
inline bool PointerProperty<int>::setIntT(int i) { return setValue(i); }
template <>
inline bool PointerProperty<t_symbol*>::setSymbolT(t_symbol* s) { return setValue(s); }
template <>
inline bool PointerProperty<Atom>::setAtomT(const Atom& a) { return setValue(a); }

}

#endif // CEAMMC_PROPERTY_H

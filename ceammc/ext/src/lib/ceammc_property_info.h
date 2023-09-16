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

#include "ceammc_atom.h"
#include "ceammc_atomlist.h"
#include "m_pd.h"

#include <boost/blank.hpp>
#include <boost/variant.hpp>
#include <cstddef>
#include <cstdint>
#include <functional>
#include <initializer_list>
#include <memory>
#include <string>
#include <vector>

namespace ceammc {

class DataTypeDict;

using PropertyValue = boost::variant<boost::blank, bool, int, t_float, t_symbol*, Atom, AtomList>;

// on order change/adding new type
// change also in to_string(PropValueType)
enum class PropValueType : uint8_t {
    BOOLEAN = 0,
    INTEGER,
    FLOAT,
    SYMBOL,
    ATOM,
    LIST
};

// on order change/adding new type
// change also in to_string(PropValueType)
enum class PropValueAccess : uint8_t {
    READONLY = 0,
    INITONLY,
    READWRITE
};

// on order change/adding new type
// change also in to_string(PropValueType)
enum class PropValueView : uint8_t {
    SLIDER = 0,
    KNOB,
    NUMBOX,
    SPINBOX,
    TOGGLE,
    MENU,
    ENTRY,
    COLOR
};

// on order change/adding new type
// change also in to_string(PropValueType)
using PropValueUnitsBase = std::uint16_t;
enum class PropValueUnits : PropValueUnitsBase {
    NONE = 0,
    MSEC = 1, // milliseconds
    SEC = 1 << 1, // seconds
    SAMP = 1 << 2, // samples
    DB = 1 << 3, // decibels
    DEG = 1 << 4, // degree
    RAD = 1 << 5, // radians
    HZ = 1 << 6, // herz
    PERCENT = 1 << 7, // percents
    CENT = 1 << 8, // cents
    SEMITONE = 1 << 9, // semitone
    TONE = 1 << 10, // tone
    BPM = 1 << 11, // bpm
    SMPTE = 1 << 12, // smpte
};

// on order change/adding new type
// change also in to_string(PropValueType)
enum class PropValueVis : uint8_t {
    PUBLIC = 0, // settable and shown in UI
    HIDDEN, // settable and hidden from UI
    INTERNAL // settable and hidden everywhere
};

// on order change/adding new type
// change also in to_string(PropValueType)
enum class PropValueConstraints : uint8_t {
    NONE = 0,
    GREATER_THEN,
    GREATER_EQUAL,
    LESS_THEN,
    LESS_EQUAL,
    CLOSED_RANGE,
    OPEN_RANGE,
    OPEN_CLOSED_RANGE,
    CLOSED_OPEN_RANGE,
    NON_ZERO,
    ENUM,
    MIN_ELEMENT_COUNT,
    MAX_ELEMENT_COUNT,
    RANGE_ELEMENT_COUNT,
    OTHER
};

t_symbol* to_symbol(PropValueType t);
t_symbol* to_symbol(PropValueView v);
t_symbol* to_symbol(PropValueUnits u);
t_symbol* to_symbol(PropValueAccess v);
t_symbol* to_symbol(PropValueVis v);
t_symbol* to_symbol(PropValueConstraints v);
inline const char* to_string(PropValueType v) { return to_symbol(v)->s_name; }
inline const char* to_string(PropValueView v) { return to_symbol(v)->s_name; }
inline const char* to_string(PropValueUnits v) { return to_symbol(v)->s_name; }
inline const char* to_string(PropValueAccess v) { return to_symbol(v)->s_name; }
inline const char* to_string(PropValueVis v) { return to_symbol(v)->s_name; }
inline const char* to_string(PropValueConstraints v) { return to_symbol(v)->s_name; }

class PropertyInfo {
    using AtomListPtr = std::unique_ptr<AtomList>;

    union NumericUnion {
        NumericUnion(t_float f_)
            : f(f_)
        {
        }

        NumericUnion(int i_)
            : i(i_)
        {
        }

        t_float f;
        int i;
    };

private:
    t_symbol* name_;
    // name
    PropertyValue default_;
    // constraints
    AtomListPtr enum_;
    NumericUnion min_, max_;
    t_float step_;
    int8_t arg_index_;
    // info
    PropValueType type_;
    PropValueUnitsBase units_;
    PropValueView view_;
    PropValueAccess access_;
    PropValueVis vis_;
    PropValueConstraints constraints_;

public:
    PropertyInfo(t_symbol* name, PropValueType type, PropValueAccess access = PropValueAccess::READWRITE);
    PropertyInfo(const std::string& name, PropValueType type, PropValueAccess access = PropValueAccess::READWRITE);
    PropertyInfo(const PropertyInfo& getTree);
    PropertyInfo(PropertyInfo&& info);
    ~PropertyInfo();

    PropertyInfo& operator=(const PropertyInfo& info);
    PropertyInfo& operator=(PropertyInfo&& info);

    t_symbol* name() const { return name_; }

    /// value info
    /// get
    inline PropValueType type() const { return type_; }
    inline PropValueView view() const { return view_; }
    inline PropValueAccess access() const { return access_; }
    inline PropValueVis visibility() const { return vis_; }
    inline PropValueConstraints constraints() const { return constraints_; }
    inline int8_t argIndex() const { return arg_index_; }
    inline bool hasArgIndex() const { return arg_index_ >= 0; }
    /// checks
    inline bool isPublic() const { return vis_ == PropValueVis::PUBLIC; }
    inline bool isHidden() const { return vis_ == PropValueVis::HIDDEN; }
    inline bool isInternal() const { return vis_ == PropValueVis::INTERNAL; }
    inline bool isReadOnly() const { return access_ == PropValueAccess::READONLY; }
    inline bool isInitOnly() const { return access_ == PropValueAccess::INITONLY; }
    inline bool isReadWrite() const { return access_ == PropValueAccess::READWRITE; }
    inline bool isBool() const { return type_ == PropValueType::BOOLEAN; }
    inline bool isFloat() const { return type_ == PropValueType::FLOAT; }
    inline bool isInt() const { return type_ == PropValueType::INTEGER; }
    inline bool isNumeric() const { return (isFloat() || isInt()); }
    inline bool isSymbol() const { return type_ == PropValueType::SYMBOL; }
    inline bool isVariant() const { return type_ == PropValueType::ATOM; }
    inline bool isList() const { return type_ == PropValueType::LIST; }

    /// set
    bool setView(PropValueView v);
    void setType(PropValueType t);
    void setAccess(PropValueAccess a);
    void setVisibility(PropValueVis v);
    bool setConstraints(PropValueConstraints c);
    void setArgIndex(int8_t idx);

    /// units
    inline PropValueUnitsBase units() const { return units_; }
    bool equalUnit(PropValueUnits u) const;
    bool hasUnit(PropValueUnits u) const;
    bool setUnits(PropValueUnits u);
    void addUnit(PropValueUnits u);
    void resetUnits() { units_ = 0; }
    void unitsIterate(const std::function<void (const char *)> &fn) const;
    void unitsIterate(const std::function<void(PropValueUnits)>& fn) const;

    /// ui hints
    // step ui hint, eg. for spinbox
    t_float step() const { return step_; }
    bool hasStep() const;
    bool setStep(t_float step);

    /// constrains
    bool hasConstraintsMin() const;
    bool hasConstraintsMax() const;
    bool hasEnumLimit() const;
    bool hasMinElementLimit() const;
    bool hasMaxElementLimit() const;
    bool hasFixedElementCount() const;
    bool hasRangeElementCount() const;

    // float range
    t_float minFloat() const { return min_.f; }
    t_float maxFloat() const { return max_.f; }
    bool setMinFloat(t_float v) CEAMMC_WARN_UNUSED;
    bool setMaxFloat(t_float v) CEAMMC_WARN_UNUSED;
    bool setRangeFloat(t_float min, t_float max) CEAMMC_WARN_UNUSED;
    void clearMinFloat();
    void clearMaxFloat();
    void clearRangeFloat();

    // int range
    int minInt() const { return min_.i; }
    int maxInt() const { return max_.i; }
    bool setMinInt(int new_min) CEAMMC_WARN_UNUSED;
    bool setMaxInt(int new_max) CEAMMC_WARN_UNUSED;
    bool setRangeInt(int min, int max) CEAMMC_WARN_UNUSED;
    void clearMinInt();
    void clearMaxInt();
    void clearRangeInt();

    // typed min/max/range
    template <typename V>
    inline bool setMinT(V v) CEAMMC_WARN_UNUSED;
    template <typename V>
    inline bool setMaxT(V v) CEAMMC_WARN_UNUSED;
    template <typename V>
    inline bool setRangeT(V min, V max) CEAMMC_WARN_UNUSED;

    // enum
    bool enumContains(t_float v) const;
    bool enumContains(int v) const;
    bool enumContains(t_symbol* s) const;
    const AtomList& enumValues() const;
    size_t enumCount() const;
    Atom enumAt(size_t idx) const;
    bool addEnum(int v) CEAMMC_WARN_UNUSED;
    bool addEnum(const char* s) CEAMMC_WARN_UNUSED;
    bool addEnum(t_symbol* s) CEAMMC_WARN_UNUSED;
    bool addEnum(const Atom& a) CEAMMC_WARN_UNUSED;
    bool addEnums(std::initializer_list<int> i_list) CEAMMC_WARN_UNUSED;
    bool addEnums(std::initializer_list<t_symbol*> args) CEAMMC_WARN_UNUSED;
    bool addEnums(std::initializer_list<const char*> c_list) CEAMMC_WARN_UNUSED;
    bool addEnums(std::initializer_list<Atom> a_list) CEAMMC_WARN_UNUSED;
    void clearEnum();

    // element count
    bool setMinElementCount(size_t n) CEAMMC_WARN_UNUSED;
    bool setMaxElementCount(size_t n) CEAMMC_WARN_UNUSED;
    bool setFixedElementCount(size_t n) CEAMMC_WARN_UNUSED;
    bool setRangeElementCount(size_t min, size_t max) CEAMMC_WARN_UNUSED;

    void setDefault(bool v);
    void setDefault(int v);
    void setDefault(size_t v);
    void setDefault(float v);
    void setDefault(double v);
    void setDefault(t_symbol* s);
    void setDefault(const Atom& a);
    void setDefault(const AtomList& lst);
    void setDefault(const PropertyValue& v);

    template <typename T>
    inline bool isA() const;
    template <typename T>
    inline bool getDefault(T& v) const
    {
        return getT<T>(default_, v);
    }

    bool noDefault() const;
    bool defaultBool(bool def = false) const;
    int defaultInt(int def = 0) const;
    t_float defaultFloat(t_float def = 0) const;
    t_symbol* defaultSymbol(t_symbol* def = &s_) const;
    Atom defaultAtom(const Atom& def = Atom()) const;
    const AtomList& defaultList() const;

    template <typename T>
    static PropValueType toType();
    template <typename T>
    static bool getT(const PropertyValue& v, T& out)
    {
        if (v.type() != typeid(T))
            return false;

        out = boost::get<T>(v);
        return true;
    }

    bool validate() const;

    bool getDict(DataTypeDict& d) const;
    bool getJSON(std::string& str) const;
};

template <>
inline bool PropertyInfo::setMinT<int>(int v) { return setMinInt(v); }
template <>
inline bool PropertyInfo::setMinT<t_float>(t_float v) { return setMinFloat(v); }

template <>
inline bool PropertyInfo::setMaxT<int>(int v) { return setMaxInt(v); }
template <>
inline bool PropertyInfo::setMaxT<t_float>(t_float v) { return setMaxFloat(v); }

template <>
inline bool PropertyInfo::setRangeT<int>(int min, int max) { return setRangeInt(min, max); }
template <>
inline bool PropertyInfo::setRangeT<t_float>(t_float min, t_float max) { return setRangeFloat(min, max); }

template <>
inline PropValueType PropertyInfo::toType<bool>() { return PropValueType::BOOLEAN; }
template <>
inline PropValueType PropertyInfo::toType<int>() { return PropValueType::INTEGER; }
template <>
inline PropValueType PropertyInfo::toType<size_t>() { return PropValueType::INTEGER; }
template <>
inline PropValueType PropertyInfo::toType<float>() { return PropValueType::FLOAT; }
template <>
inline PropValueType PropertyInfo::toType<double>() { return PropValueType::FLOAT; }
template <>
inline PropValueType PropertyInfo::toType<t_symbol*>() { return PropValueType::SYMBOL; }
template <>
inline PropValueType PropertyInfo::toType<Atom>() { return PropValueType::ATOM; }
template <>
inline PropValueType PropertyInfo::toType<AtomList>() { return PropValueType::LIST; }

template <>
inline bool PropertyInfo::isA<bool>() const { return isBool(); }
template <>
inline bool PropertyInfo::isA<t_float>() const { return isFloat(); }
template <>
inline bool PropertyInfo::isA<int>() const { return isInt(); }
template <>
inline bool PropertyInfo::isA<t_symbol*>() const { return isSymbol(); }
template <>
inline bool PropertyInfo::isA<Atom>() const { return isVariant(); }
template <>
inline bool PropertyInfo::isA<AtomList>() const { return isList(); }

template <>
inline bool PropertyInfo::getT<AtomList>(const PropertyValue& v, AtomList& out)
{
    if (v.type() != typeid(AtomList))
        return false;

    out = boost::get<AtomList>(v);
    return true;
}

}

#endif // CEAMMC_PROPERTY_INFO_H

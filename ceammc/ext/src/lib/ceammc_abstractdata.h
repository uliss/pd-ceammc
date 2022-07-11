/*****************************************************************************
 * Copyright 2017 Serge Poltavsky. All rights reserved.
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
#ifndef CEAMMC_ABSTRACTDATA_H
#define CEAMMC_ABSTRACTDATA_H

#include "ceammc_atom.h"
#include "ceammc_datatypes.h"

namespace ceammc {

class AtomListView;

/**
 * @brief Base class for all data types
 */
class AbstractData {
public:
    AbstractData() = default;
    AbstractData(const AbstractData&) = delete;

    virtual ~AbstractData() noexcept;

    /**
     * override this method to dump data to Pd console
     */
    virtual void dump() const;

    /**
     * This method should return pointer to new dynamically allocated copy of data
     */
    virtual AbstractData* clone() const = 0;

    /**
     * This method should return ID to data type.
     */
    virtual DataTypeId type() const noexcept = 0;

    /**
     * return object content for list constructor
     */
    virtual std::string toListStringContent() const = 0;

    /**
     * return object content for data constructor
     */
    virtual std::string toDictStringContent() const = 0;

    /**
     * return object representation for data string list constructor
     */
    std::string toListString() const;

    /**
     * return object representation for data string list constructor
     */
    std::string toDictString() const;

    /**
     * String data representation for converting to String() datatype
     */
    virtual std::string toString() const;

    /**
     * Returns JSON object with "datatype" and "value" fields
     */
    std::string toJsonObject() const;

    /**
     * Returns object value as JSON string
     */
    virtual std::string toJsonString() const;

    /**
     * Set data from string constructor
     * @return false on error
     */
    virtual bool setFromDataString(const std::string& str);

    /**
     * Set data from args constructor
     * @return false on error
     */
    virtual bool setFromDataList(const AtomListView& lv);

    /**
     * Set data from (data)atom
     * @return true on sucess, false on error
     */
    virtual bool setFromAtom(const Atom& a);

    /**
     * virtual assign function: should set with same datatype only
     * @param d - pointer to real data type
     * @return true on success, false on error
     */
    virtual bool set(const AbstractData* d) noexcept = 0;

    /**
     * Override this method to compare data by pointer to base class
     */
    virtual bool isEqual(const AbstractData* d) const noexcept;

    /**
     * Override this method to compare data by pointer to base class
     */
    virtual bool isLess(const AbstractData* d) const noexcept;

    /**
     * Returns datatype name
     */
    virtual DataTypeName typeName() const noexcept final;

    /**
     * Helper functions to return pointer to derived classes
     */
    template <class T>
    T* as();
    template <class T>
    const T* as() const;

    /**
     * Typed version of clone() method
     */
    template <class T>
    T* cloneT() const;

    /**
     * Check if data have list constructor like DataName(args...)
     */
    virtual bool canInitWithList() const noexcept;

    /**
     * Check if data have dict constructor like DataName[key: args...]
     */
    virtual bool canInitWithDict() const noexcept;

    /**
     * Assign function: set data from pointer to other data
     * @param d - pointer to real data
     * @return true on success, false on error
     */
    template <typename T>
    bool setDataT(const AbstractData* d) noexcept
    {
        if (!d || d->type() != T::dataType)
            return false;

        auto t = static_cast<const T*>(d);
        *static_cast<T*>(this) = *t;
        return true;
    }

public:
    /**
     * Parse data string for desired datatype
     * @param str - input string
     * @param dataType - desired datatype
     * @param res - where to store result if parsing is ok
     * @return true on success, false on error
     * @note writes error message to Pd Console window
     */
    static bool parse(const std::string& str, int dataType, Atom& res);

    /**
     * Parse data atomlist for desired datatype
     * @param lv - input list
     * @param dataType - desired datatype
     * @param res - where to store result if parsing is ok
     * @return true on success, false on error
     * @note writes error message to Pd Console window
     */
    static bool parse(const AtomListView& lv, int dataType, Atom& res);

    template <typename T>
    bool parseT(const std::string& str, Atom& res) { return parse(str, T::dataType, res); }
    template <typename T>
    bool parseT(const AtomListView& lv, Atom& res) { return parse(lv, T::dataType, res); }

    static DataTypeName findTypeName(DataTypeId dataType);
};

template <class T>
T* AbstractData::cloneT() const
{
    if (type() != T::dataType)
        return nullptr;

    return static_cast<T*>(clone());
}

template <class T>
T* AbstractData::as()
{
    return type() == T::dataType ? static_cast<T*>(this) : nullptr;
}

template <class T>
const T* AbstractData::as() const
{
    return type() == T::dataType ? static_cast<const T*>(this) : nullptr;
}
}

#endif // CEAMMC_ABSTRACTDATA_H

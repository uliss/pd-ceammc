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
#ifndef CEAMMC_XDATA_H
#define CEAMMC_XDATA_H

#include "ceammc_abstractdata.h"

#include <type_traits>

namespace ceammc {

class DataPtr {
    DataDesc desc_;
    const AbstractData* data_;

public:
    /**
     * Creates new data pointer (should be allocated via new)
     * Takes ownership on the specified pointer
     * @param data - data pointer (if nullptr given returns null DataPtr)
     */
    DataPtr(AbstractData* data);

    /**
     * Unpacks pointer from atom
     * If atom is not a pointer - null DataPtr is created
     */
    DataPtr(const Atom& data);

    // copy/move
    DataPtr(const DataPtr& d);
    DataPtr(DataPtr&& d);
    DataPtr& operator=(const DataPtr& d);
    DataPtr& operator=(DataPtr&& d);

    /**
     * Removes owned data if reference counter equal 1
     */
    ~DataPtr();

    bool isValid() const;
    bool isNull() const { return !isValid(); }

    /**
     * Returns pointed data description
     */
    const DataDesc& desc() const { return desc_; }

    /**
     * Number of references pointed data
     */
    size_t refCount() const;

    /**
     * Returns data pointer
     */
    const AbstractData* data() const { return data_; }
    const AbstractData* operator->() const { return data_; }

    /**
     * Return pointer to specified data type
     * On type mismatch returns nullptr
     */
    template <class T>
    const T* as() const;

    /**
     * Return data pointer packed as Atom
     */
    Atom asAtom() const { return Atom(desc_); }

    /**
     * compare pointed data
     */
    bool operator==(const DataPtr& d) const;
    bool operator!=(const DataPtr& d) const { return !this->operator==(d); }

protected:
    void invalidate();
};

template <class T>
const T* DataPtr::as() const
{
    if (data_ == 0)
        return 0;

    return data_->as<T>();
}

/// compare operator
bool operator<(const DataPtr& d0, const DataPtr& d1);

/// output operator
std::ostream& operator<<(std::ostream& os, const DataPtr& d0);

template <class T>
class DataTPtr : public DataPtr {
public:
    /// move constructor
    DataTPtr(DataTPtr&& ptr)
        : DataPtr(std::move(ptr))
    {
        static_assert(std::is_base_of<AbstractData, T>::value, "Should be base of AbstractData");
    }

    /// copy constructor
    DataTPtr(const DataTPtr& ptr)
        : DataPtr(ptr)
    {
        static_assert(std::is_base_of<AbstractData, T>::value, "Should be base of AbstractData");
    }

    /// assign move operator
    DataTPtr& operator=(DataTPtr&& d)
    {
        DataPtr::operator=(std::move(d));
        return *this;
    }

    /// assign copy operator
    DataTPtr& operator=(const DataTPtr& d)
    {
        DataPtr::operator=(d);
        return *this;
    }

    /// create from pointer
    /// take ownership
    DataTPtr(T* ptr)
        : DataPtr(ptr)
    {
    }

    /// create from data copy/move
    DataTPtr(T&& d)
        : DataPtr(new T(std::forward<T>(d)))
    {
    }

    /// unpack from atom
    DataTPtr(const Atom& a)
        : DataPtr(a.isDataType(T::dataType) ? a : Atom())
    {
    }

    /// equal operator
    bool operator==(const DataTPtr& d) const
    {
        if (data() == d.data())
            return true;

        if (isValid() && d.isValid())
            return *data() == *d.data();

        return false;
    }

    /// data access
    const T* data() const { return static_cast<const T*>(DataPtr::data()); }
    const T* operator->() const { return data(); }
    const T& operator*() const { return *data(); }
};
}

#endif // CEAMMC_XDATA_H

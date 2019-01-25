/*
 * The MIT License (MIT)
 *
 * Copyright (C) 2018-2019 Alex Nadzharov, Serge Poltavsky
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */

// template class to wrap class into AbstractData
#ifndef WRAPPER_DATATYPE_H
#define WRAPPER_DATATYPE_H

#include "ceammc_abstractdata.h"
#include "ceammc_data.h"
#include "ceammc_datatypes.h"

#include <algorithm>
#include <memory>
#include <string>
#include <type_traits>

using namespace ceammc;

namespace wrapper {

class WrapperIDFactory {
    size_t id_;

private:
    WrapperIDFactory();
    WrapperIDFactory(const WrapperIDFactory&) = delete;
    void operator=(const WrapperIDFactory&) = delete;

public:
    static WrapperIDFactory& instance();
    size_t generateNewId();
};

class AbstractDataId : public AbstractData {
    size_t data_type_id_;

public:
    AbstractDataId(size_t id);
    size_t dataTypeId() const;
    bool hasEqualType(const AbstractData* d) const;
};

template <typename T>
class AbstractDataWrapper : public AbstractDataId {
    T value_;

public:
    explicit AbstractDataWrapper(const T& v)
        : AbstractDataId(wrappedDataTypeId)
        , value_(v)
    {
    }

    AbstractDataWrapper()
        : AbstractDataId(wrappedDataTypeId)
    {
    }

    AbstractDataWrapper& operator=(const AbstractDataWrapper& v)
    {
        value_ = v.value_;
        return (*this);
    }

    AbstractDataWrapper* clone() const override
    {
        return new AbstractDataWrapper(value_);
    }

    bool isEqual(const AbstractData* d) const override
    {
        if (!hasEqualType(d))
            return false;

        auto* dw = static_cast<const AbstractDataWrapper*>(d);
        return value_ == dw->value_;
    }

    const T& value() const { return value_; }
    T& value() { return value_; }
    DataType type() const override { return AbstractDataWrapper<T>::dataType; }
    std::string toString() const override { return value_.toString(); }

public:
    static const DataType dataType;
    static const size_t wrappedDataTypeId;
};

template <typename T>
const DataType AbstractDataWrapper<T>::dataType = ceammc::data::DATA_WRAPPER;

template <typename T>
const size_t AbstractDataWrapper<T>::wrappedDataTypeId = WrapperIDFactory::instance().generateNewId();

template <class T>
class WrapperTPtr : public DataTPtr<AbstractDataWrapper<T>> {
    using Type = AbstractDataWrapper<T>;

public:
    WrapperTPtr(AbstractDataWrapper<T>* d)
        : DataTPtr<Type>(d)
    {
    }

    WrapperTPtr(const Atom& a)
        : DataTPtr<Type>(a)
    {
        if (!a.isDataType(Type::dataType)) {
            this->invalidate();
            return;
        }

        DataPtr dptr(a);
        if (dptr.isNull()) {
            this->invalidate();
            return;
        }

        const AbstractDataId* id = dynamic_cast<const AbstractDataId*>(dptr.data());
        if (!id) {
            this->invalidate();
            return;
        }

        if (id->dataTypeId() != Type::wrappedDataTypeId) {
            this->invalidate();
            return;
        }
    }
};

}

#endif // WRAPPER_DATATYPE_H

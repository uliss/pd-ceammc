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
#include "ceammc_datastorage.h"
#include "ceammc_log.h"

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
    AbstractDataId(const AbstractDataId& id)
        : data_type_id_(id.data_type_id_)
    {
    }

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

    AbstractDataWrapper(const AbstractDataWrapper& v)
        : AbstractDataId(v)
        , value_(v.value_)
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

    bool isEqual(const AbstractData* d) const noexcept override
    {
        if (!hasEqualType(d))
            return false;

        auto* dw = static_cast<const AbstractDataWrapper*>(d);
        return value_ == dw->value_;
    }

    const T& value() const noexcept { return value_; }
    T& value() noexcept { return value_; }
    int type() const noexcept override { return AbstractDataWrapper<T>::dataType; }
    std::string toString() const override { return value_.toString(); }

public:
    static const int dataType;
    static const size_t wrappedDataTypeId;
};

template <typename T>
const int AbstractDataWrapper<T>::dataType = ceammc::DataStorage::instance().registerNewType(
    T::typeName(),
    [](const AtomList& lst) -> AbstractData* {
        T data;
        auto st = data.setFromPd(lst);
        std::string err;
        if (st.error(&err)) {
            LIB_ERR << err;
            return nullptr;
        } else {
            return new AbstractDataWrapper(data);
        }
    });

template <typename T>
const size_t AbstractDataWrapper<T>::wrappedDataTypeId = WrapperIDFactory::instance().generateNewId();

template <typename T>
using WrapperTPtr = DataAtom<AbstractDataWrapper<T>>;

}

#endif // WRAPPER_DATATYPE_H

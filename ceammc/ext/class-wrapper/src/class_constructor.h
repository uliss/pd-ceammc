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

// creates Pd object for class
#ifndef CLASS_CONSTRUCTOR_HPP
#define CLASS_CONSTRUCTOR_HPP

#include "ceammc_atomlist.h"
#include "ceammc_factory.h"
#include "ceammc_object.h"

#include "wrapper_datatype.h"
#include "wrapper_fn_traits.h"
#include "wrapper_invocations.h"
#include "wrapper_sequence.h"
#include "wrapper_tuple.h"

using namespace ceammc;

namespace wrapper {

template <typename T>
class ClassConstructorCustom : public ceammc::BaseObject {
public:
    using TypeWrapped = AbstractDataWrapper<T>;
    using TypedDataPtr = WrapperTPtr<T>;

private:
    TypedDataPtr data_;

public:
    explicit ClassConstructorCustom(PdArgs& a)
        : BaseObject(a)
        , data_(new TypeWrapped())
    {
        static t_symbol* SYM_T = gensym(T::typeName());

        if (!positionalArguments().empty()) {
            T data;
            auto st = data.setFromPd(positionalArguments(), SYM_T);
            std::string err;
            if (st.error(&err)) {
                OBJ_ERR << err;
            } else
                data_ = TypedDataPtr(new TypeWrapped(data));
        }

        createOutlet();
    }

    void onBang() override
    {
        dataTo(0, data_);
    }

    void onFloat(t_float f) override
    {
        onList(AtomList(f));
    }

    void onSymbol(t_symbol* s) override
    {
        onList(AtomList(s));
    }

    void onList(const AtomList& l) override
    {
        T data;
        auto st = data.setFromPd(l);
        std::string err;
        if (!st.error(&err)) {
            data_ = TypedDataPtr(new TypeWrapped(data));
            dataTo(0, data_);
        } else {
            OBJ_ERR << err;
        }
    }

    void onAny(t_symbol* s, const AtomList& l) override
    {
        T data;
        auto st = data.setFromAny(s, l);
        std::string err;
        if (!st.error(&err)) {
            data_ = TypedDataPtr(new TypeWrapped(data));
            dataTo(0, data_);
        } else {
            OBJ_ERR << err;
        }
    }

    void onDataT(const DataAtom<TypeWrapped>& dptr)
    {
        if (dptr->dataTypeId() != TypeWrapped::wrappedDataTypeId) {
            OBJ_ERR << "unexpected data with id=" << dptr->dataTypeId()
                    << ", expecting " << T::typeName()
                    << " with id=" << TypeWrapped::wrappedDataTypeId;
            return;
        }

        data_ = TypedDataPtr(dptr->clone());
        dataTo(0, data_);
    }

    void m_set(t_symbol* s, const AtomList& l)
    {
        if (l.isData()) {
            TypedDataPtr ptr(l.at(0));
            if (!ptr.isValid())
                METHOD_ERR(s) << "invalid data pointer";

            data_ = ptr;
        } else {
            T data;
            auto st = data.setFromPd(l);
            std::string err;
            if (!st.error(&err))
                data_ = TypedDataPtr(new TypeWrapped(data));
            else
                METHOD_ERR(s) << err;
        }
    }

    void dump() const override
    {
        BaseObject::dump();
        OBJ_DBG << "value: " << data_->toString();
    }

    const TypedDataPtr& data() const
    {
        return data_;
    }
};

}

#endif

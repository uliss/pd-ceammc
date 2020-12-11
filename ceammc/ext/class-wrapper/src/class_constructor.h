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
    using TypedDataAtom = DataAtom<TypeWrapped>;

private:
    TypedDataAtom data_;

public:
    explicit ClassConstructorCustom(PdArgs& a)
        : BaseObject(a)
    {
        if (parsedPosArgs().isData()) {
            if (!parsedPosArgs().template isA<TypeWrapped>())
                OBJ_ERR << "unsupported init data type: " << parsedPosArgs()[0].asData()->typeName();
            else
                data_ = TypedDataAtom(parsedPosArgs()[0]);
        } else {
            T data;
            auto res = data.setFromPd(parsedPosArgs());
            std::string err;
            if (res.error(&err))
                OBJ_ERR << err;
            else
                data_ = TypedDataAtom(data);
        }

        createOutlet();
    }

    void onBang() override
    {
        atomTo(0, data_);
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
            data_ = TypedDataAtom(data);
            atomTo(0, data_);
        } else {
            OBJ_ERR << err;
        }
    }

    void onAny(t_symbol* s, const AtomListView& l) override
    {
        T data;
        auto st = data.setFromAny(s, l);
        std::string err;
        if (!st.error(&err)) {
            data_ = TypedDataAtom(data);
            atomTo(0, data_);
        } else {
            OBJ_ERR << err;
        }
    }

    void onDataT(const TypedDataAtom& data)
    {
        data_ = data;
        atomTo(0, data_);
    }

    void m_set(t_symbol* s, const AtomListView& l)
    {
        if (l.isData()) {
            if (l.isA<TypeWrapped>()) {
                auto d = l.asD<TypeWrapped>();
                data_ = TypedDataAtom(*d);
            }
        } else {
            T data;
            auto st = data.setFromPd(l);
            std::string err;
            if (!st.error(&err))
                data_ = TypedDataAtom(data);
            else
                METHOD_ERR(s) << err;
        }
    }

    void dump() const override
    {
        BaseObject::dump();
        OBJ_DBG << "value: " << data_->toString();
    }

    const TypedDataAtom& data() const
    {
        return data_;
    }
};

}

#endif

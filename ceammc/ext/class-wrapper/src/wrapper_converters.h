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

#ifndef WRAPPER_CONVERTERS_H
#define WRAPPER_CONVERTERS_H

#include "ceammc_atomlist.h"
#include "ceammc_data.h"
#include "wrapper_datatype.h"

namespace wrapper {

template <typename T>
class AtomListFromReturnType {
    DataAtom<AbstractDataWrapper<T>> dptr_;

public:
    explicit AtomListFromReturnType(T v)
        : dptr_(v)
    {
    }

    AtomListFromReturnType()
        : dptr_()
    {
    }

    AtomList asAtomList() const
    {
        AtomList ret;

        if (!dptr_.data())
            return AtomList(Atom(gensym("<empty>")));

        //        toAtomList(ret, dptr_.data()->value());

        return ret;
    }
};

template <>
class AtomListFromReturnType<void> : public Atom {
public:
    AtomListFromReturnType(void* v = 0) {}

    AtomList asAtomList() const
    {
        return AtomList();
    }
};

}

#endif // WRAPPER_CONVERTERS_H

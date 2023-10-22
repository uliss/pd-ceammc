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

#ifndef CLASS_FUNCTION_H
#define CLASS_FUNCTION_H

#include "ceammc_object.h"

#include "wrapper_fn_traits.h"

#include <type_traits>

using namespace ceammc;

namespace wrapper {

template <typename F>
class ClassFunctionBasePtr : public BaseObject {
    F fn_;

public:
    using Traits = FunctionTraits<F>;

public:
    ClassFunctionBasePtr(const PdArgs& args, F fn)
        : BaseObject(args)
        , fn_(fn)
    {
        createOutlet();
    }
};

template <typename F, typename Args>
class ClassFunctionBase : public ClassFunctionBasePtr<F> {
public:
    ClassFunctionBase(const PdArgs& args, F fn)
        : ClassFunctionBasePtr<F>(args, fn)
    {
    }
};

template <typename F>
class ClassFunctionBase<F, std::tuple<int>> : public ClassFunctionBasePtr<F> {
public:
    ClassFunctionBase(const PdArgs& args, F fn)
        : ClassFunctionBasePtr<F>(args, fn)
    {
    }

    void onInlet(t_float f)
    {
    }
};

template <typename F>
class ClassFunctionBase<F, std::tuple<float>> : public ClassFunctionBasePtr<F> {
public:
    ClassFunctionBase(const PdArgs& args, F fn)
        : ClassFunctionBasePtr<F>(args, fn)
    {
    }

    void onInlet(t_float f)
    {
    }
};

template <typename F>
class ClassFunction : public ClassFunctionBase<F, typename FunctionTraits<F>::arguments> {

public:
    ClassFunction(const PdArgs& args, F fn)
        : ClassFunctionBase<F, typename FunctionTraits<F>::arguments>(args, fn)
    {
    }
};
}

#endif // CLASS_FUNCTION_H

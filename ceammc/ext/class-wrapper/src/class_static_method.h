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

// creates Pd object for static method

#ifndef CLASS_STATIC_METHOD_HPP
#define CLASS_STATIC_METHOD_HPP

#include "ceammc_atomlist.h"
#include "ceammc_dataatom.h"
#include "ceammc_log.h"
#include "ceammc_object.h"

#include "wrapper_converters.h"
#include "wrapper_fn_traits.h"
#include "wrapper_invocations.h"
#include "wrapper_output.h"
#include "wrapper_sequence.h"
#include "wrapper_tuple.h"

#include <functional>

namespace wrapper {

template <typename T, class Method>
class ClassStaticMethod : public ceammc::BaseObject {
public:
    using MethodTraits = FunctionTraits<Method>;
    using MethodArgs = typename MethodTraits::arguments;
    using MethodReturnType = typename MethodTraits::return_type;
    using MethodCall = InvocationStaticMethod<MethodReturnType, Method, MethodArgs>;
    using InletArgSetter = InletArgFromAtomList<MethodTraits::nargs, MethodArgs>;

    static constexpr bool has_varargs = tuple_utils::has_type<std::vector<float>, MethodArgs>::value
        || tuple_utils::has_type<std::vector<int>, MethodArgs>::value
        || tuple_utils::has_type<std::vector<std::string>, MethodArgs>::value
        || tuple_utils::has_type<AtomList, MethodArgs>::value;

private:
    Method method_;
    MethodArgs arguments_;

public:
    ClassStaticMethod(PdArgs& a, Method m)
        : BaseObject(a)
        , method_(m)
    {
        initXlets();
        initArguments();
    }

    void initXlets()
    {
        for (size_t i = 1; i < MethodTraits::nargs; i++)
            createInlet();

        for (size_t i = 0; i < MethodTraits::nouts; i++)
            createOutlet();
    }

    void initArguments()
    {
        try {
            atomListToArguments<Method>(positionalArguments(), arguments_);
        } catch (std::exception& e) {
            OBJ_ERR << "initial arguments: " << e.what();
        }
    }

    void dispatch()
    {
        try {
            MethodCall call(method_, arguments_);
            call.exec();
            outputTo<MethodReturnType>(0, call.result_);
        } catch (std::exception& e) {
            OBJ_ERR << e.what();
        }
    }

    void onBang() override
    {
        dispatch();
    }

    void onFloat(float f) override
    {
        switch (MethodTraits::nargs) {
        case 0:
            OBJ_ERR << "unexpected float: " << f;
            return;
        case 1:
            onList(AtomList(Atom(f)));
            break;
        default: {
            InletArgSetter arg_setter(arguments_);
            // note: InletArgSetter is designed for ClassMethod and has 1-based argument count
            // so we have to do correction by +1
            ErrorMsg err = arg_setter.setNthArg(1, AtomList(Atom(f)));
            if (err)
                OBJ_ERR << err.msg();
            dispatch();
        }
        }
    }

    void onSymbol(t_symbol* s) override
    {
        onList(AtomList(Atom(s)));
    }

    void onList(const AtomList& l) override
    {
        if (!processList(l))
            return;

        dispatch();
    }

    void onInlet(size_t n, const AtomList& lst) override
    {
        InletArgSetter arg_setter(arguments_);
        // note: InletArgSetter is designed for ClassMethod and has 1-based argument count
        // so we have to do correction by +1
        ErrorMsg err = arg_setter.setNthArg(n + 1, lst);
        if (err)
            OBJ_ERR << err.msg();
    }

    void dump() const override
    {
        BaseObject::dump();
        Debug dbg(this);
        dbg << "args:";
        tuple_utils::for_each(arguments_, PdDump(dbg));
    }

private:
    template <typename C>
    void outputTo(size_t n, const C& v)
    {
        using ThisType = ClassStaticMethod<T, Method>;
        PdOutput<ThisType, C> out(this);
        out.output(n, v);
    }

    template <bool enable = has_varargs>
    bool processList(const typename std::enable_if<enable, AtomList>::type& l)
    {
        try {
            atomListToArguments<Method>(l, arguments_);
            return true;
        } catch (std::exception& e) {
            OBJ_ERR << "initial arguments: " << e.what();
            return false;
        }
    }

    template <bool enable = has_varargs>
    bool processList(const typename std::enable_if<!enable, AtomList>::type& l)
    {
        if (l.size() != MethodTraits::nargs) {
            OBJ_ERR << "bad message: expecting "
                    << MethodTraits::nargs << " arguments, but "
                    << l.size() << " was provided";
            return false;
        }

        try {
            atomListToArguments<Method>(l, arguments_);
            return true;
        } catch (std::exception& e) {
            OBJ_ERR << "initial arguments: " << e.what();
            return false;
        }
    }
};
}

#endif // CLASS_STATIC_METHOD_HPP

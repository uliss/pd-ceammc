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

// creates Pd object for class method
#ifndef WRAPPER_CLASS_METHOD_H
#define WRAPPER_CLASS_METHOD_H

#include "data_iface.h"
#include "wrapper_fn_traits.h"
#include "wrapper_invocations.h"
#include "wrapper_output.h"
#include "wrapper_tuple.h"

#include "ceammc_atomlist.h"
#include "ceammc_log.h"
#include "ceammc_object.h"

#include <exception>
#include <sstream>
#include <string>

namespace wrapper {

template <class Data, class External>
class OverloadDataCall {
    Data* data_;
    External* external_;
    const AtomList& l_;
    int call_idx_;

public:
    OverloadDataCall(int idx, const AtomList& l, Data* data, External* ext)
        : data_(data)
        , external_(ext)
        , l_(l)
        , call_idx_(idx)
    {
    }

    template <typename T, typename MemFn>
    bool operator()(int index, T&& v, MemFn&& fn)
    {
        if (index == call_idx_) {
            callMethod(v, fn);
            return true;
        }

        return false;
    }

    template <typename MemFn>
    void callMethod(std::tuple<float>& d, MemFn&& fn)
    {
        outputTo(0, (data_->*fn)(std::get<0>(d)));
    }

    template <typename MemFn>
    void callMethod(std::tuple<>& d, MemFn&& fn)
    {
        outputTo(0, (data_->*fn)());
    }

    template <typename T, typename MemFn>
    void callMethod(T&& t, MemFn&& fn)
    {
        outputTo(0, (data_->*fn)(std::get<0>(t)));
    }

    template <typename C>
    void outputTo(size_t n, const C& v)
    {
        PdOutput<External, C> out(external_);
        out.output(n, v);
    }
};

template <typename T, typename MethodList>
class ClassMethod : public BaseObject {
public:
    using MethodOverloadList = typename tuple_utils::tail<MethodList>::type;
    using Method = typename tuple_utils::head<MethodList>::type;
    using MethodTraits = FunctionTraits<Method>;
    using MethodArgs = typename MethodTraits::arguments;
    using MethodReturnType = typename MethodTraits::return_type;
    using DataType = AbstractDataWrapper<T>;
    using TypedDataPtr = DataAtom<DataType>;
    using InletArgSetter = InletArgFromAtomList<MethodTraits::nargs, MethodArgs>;
    using MethodCall = InvocationClassMethod<typename MethodTraits::return_type, T, Method, MethodArgs>;
    using MethodOverloadedArgs = typename MethodArgList<MethodOverloadList>::type;

private:
    T data_;
    Method main_method_;
    MethodArgs main_method_args_;
    MethodOverloadList overloads_;
    MethodOverloadedArgs overload_args_;
    int method_call_idx_;
    ListProperty* pd_args_;
    static const int MAIN_METHOD_CALL = -1;

public:
    ClassMethod(PdArgs& a, MethodList methods)
        : BaseObject(a)
        , main_method_(std::get<0>(methods))
        , overloads_(tuple_utils::get_tail(methods))
        , method_call_idx_(MAIN_METHOD_CALL)
        , pd_args_(nullptr)
    {
        initXlets();
        initArguments();
    }

    void initArguments()
    {
        pd_args_ = new ListProperty("@args");
        pd_args_->setArgIndex(0);

        pd_args_->setListCheckFn([this](const AtomList& args) -> bool {
            // first try to find appropriate overloaded method
            int idx = tuple_utils::find_first(overload_args_, ArgumentMatchAndSet(args));
            if (idx >= 0) {
                method_call_idx_ = idx;
                return true;
            }

            // unexpected arguments warning
            if (MethodTraits::nargs == 0 && args.size() > 0) {
                OBJ_ERR << "no arguments expected: " << args;
                return false;
            }

            try {
                atomListToArguments<Method>(args, main_method_args_);
                return true;
            } catch (std::exception& e) {
                OBJ_ERR << "initial arguments error: " << e.what();
                return false;
            }
        });

        addProperty(pd_args_);
    }

    void onInlet(size_t n, const AtomList& lst) override
    {
        int idx = tuple_utils::find_first(overload_args_, ArgumentMatchAndSet(lst));
        if (idx >= 0) {
            method_call_idx_ = idx;
            return;
        }

        InletArgSetter arg_setter(main_method_args_);
        ErrorMsg err = arg_setter.setNthArg(n, lst);
        if (err)
            OBJ_ERR << err.msg();

        method_call_idx_ = MAIN_METHOD_CALL;
    }

    void onDataT(const DataAtom<DataType>& data)
    {
        if (data->dataTypeId() != DataType::wrappedDataTypeId) {
            OBJ_ERR << "unexpected data with id=" << data->dataTypeId()
                    << ", expecting " << T::typeName()
                    << " with id=" << DataType::wrappedDataTypeId;
            return;
        }

        data_ = data->value();
        dispatch();
    }

    void onList(const AtomList& lst) override
    {
        Result res = data_.setFromList(lst);

        if (res.isOk()) {
            dispatch();
        } else {
            std::string msg;
            res.error(&msg);
            OBJ_ERR << msg;
        }
    }

    void onFloat(t_float f) override
    {
        Result res = data_.setFromFloat(f);

        switch (res.code()) {
        case DataIFace::ERROR: {
            std::string msg;
            res.error(&msg);
            OBJ_ERR << msg;
            return;
        } break;
        case DataIFace::NOT_SUPPORTED:
            // try from list
            return onList(Atom(f));
            break;
        case DataIFace::OK:
            dispatch();
            break;
        }
    }

    void onSymbol(t_symbol* s) override
    {
        Result res = data_.setFromSymbol(s);

        switch (res.code()) {
        case DataIFace::ERROR: {
            std::string msg;
            res.error(&msg);
            OBJ_ERR << msg;
            return;
        } break;
        case DataIFace::NOT_SUPPORTED:
            // try from list
            return onList(Atom(s));
            break;
        case DataIFace::OK:
            dispatch();
            break;
        }
    }

    void onAny(t_symbol* s, const AtomList& lst) override
    {
        Result res = data_.setFromAny(s, lst);

        if (res.isOk()) {
            dispatch();
        } else {
            std::string msg;
            res.error(&msg);
            OBJ_ERR << msg;
        }
    }

    void dump() const override
    {
        BaseObject::dump();

        OBJ_DBG << "name:    " << T::typeName();
        OBJ_DBG << "type id: " << DataType::wrappedDataTypeId;
        OBJ_DBG << "value:   " << data_.toString();

        Debug dbg(this);
        dbg << "args:";
        tuple_utils::for_each(main_method_args_, PdDump(dbg));
    }

private:
    template <typename C>
    void outputTo(size_t n, const C& v)
    {
        using ThisType = typename std::remove_reference<decltype(*this)>::type;
        PdOutput<ThisType, C> out(this);
        out.output(n, v);
    }

    void initXlets()
    {
        for (size_t i = 0; i < MethodTraits::nargs; i++)
            createInlet();

        for (size_t i = 0; i < MethodTraits::nouts; i++)
            createOutlet();
    }

    void dispatch()
    {
        try {
            if (method_call_idx_ != MAIN_METHOD_CALL) {
                using ThisT = typename std::remove_reference<decltype(*this)>::type;

                OverloadDataCall<T, ThisT> call(method_call_idx_, pd_args_->value(), &data_, this);
                if (tuple_utils::find_first(overload_args_, overloads_, call) != -1)
                    return;

                OBJ_ERR << "non dispatched method: " << method_call_idx_;
                return;
            }

            MethodCall call(data_, main_method_, main_method_args_);
            call.exec();
            outputTo<MethodReturnType>(0, call.result);
        } catch (std::exception& e) {
            OBJ_ERR << e.what();
        }
    }
};
}

#endif // WRAPPER_CLASS_METHOD_H

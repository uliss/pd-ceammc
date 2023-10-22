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

#ifndef WRAPPER_INVOCATIONS_H_
#define WRAPPER_INVOCATIONS_H_

#include "wrapper_sequence.h"

#include <memory>
#include <utility>

namespace wrapper {

template <typename R, class C, class F, typename Args>
class InvocationClassMethod {
    C& class_;
    const F& method_;
    const Args& arguments_;

public:
    R result;

public:
    InvocationClassMethod(const C& c, const F& f, const Args& a)
        : class_(const_cast<C&>(c))
        , method_(f)
        , arguments_(a)
    {
    }

    void exec()
    {
        constexpr size_t N = std::tuple_size<Args>::value;
        operator()(typename GenSequence<N>::type());
    }

    template <int... S>
    void operator()(IntSequence<S...>)
    {
        result = (class_.*method_)(std::get<S>(arguments_)...);
    }
};

// void return type
template <class C, class F, typename Args>
class InvocationClassMethod<void, C, F, Args> {
    C& class_;
    const F& method_;
    const Args& arguments_;

public:
    InvocationClassMethod(const C& c, const F& f, const Args& a)
        : class_(const_cast<C&>(c))
        , method_(f)
        , arguments_(a)
    {
    }

    void exec()
    {
        constexpr size_t N = std::tuple_size<Args>::value;
        operator()(typename GenSequence<N>::type());
    }

    template <int... S>
    void operator()(IntSequence<S...>)
    {
        (class_.*method_)(std::get<S>(arguments_)...);
    }
};

// ---

template <typename R, class F, typename Args>
class InvocationStaticMethod {
    const F& method_;
    const Args& arguments_;

public:
    R result_;

    InvocationStaticMethod(const F& f, const Args& a)
        : method_(f)
        , arguments_(a)
    {
    }

    void exec()
    {
        constexpr size_t N = std::tuple_size<Args>::value;
        operator()(typename GenSequence<N>::type());
    }

    template <int... S>
    void operator()(IntSequence<S...>)
    {
        result_ = (*method_)(std::get<S>(arguments_)...);
    }
};

// void return type
template <class F, typename Args>
class InvocationStaticMethod<void, F, Args> {
    const F& method_;
    const Args& arguments_;

public:
    InvocationStaticMethod(const F& f, const Args& a)
        : method_(f)
        , arguments_(a)
    {
    }

    void exec()
    {
        constexpr size_t N = std::tuple_size<Args>::value;
        operator()(typename GenSequence<N>::type());
    }

    template <int... S>
    void operator()(IntSequence<S...>)
    {
        (*method_)(std::get<S>(arguments_)...);
    }
};

}

#endif // WRAPPER_INVOCATIONS_H_

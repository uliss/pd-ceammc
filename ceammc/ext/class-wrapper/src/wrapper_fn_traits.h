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

// function traits

#ifndef WRAPPER_FN_TRAITS_H
#define WRAPPER_FN_TRAITS_H

#include <cstddef>
#include <tuple>
#include <type_traits>

namespace wrapper {

template <typename ReturnType>
struct ReturnTypeSize {
    static constexpr std::size_t value = 1;
};

template <>
struct ReturnTypeSize<void> {
    static constexpr std::size_t value = 0;
};

template <typename... Args>
struct ReturnTypeSize<std::tuple<Args...>> {
    static constexpr std::size_t value = sizeof...(Args);
};

template <class F>
struct FunctionTraits;

// function pointer
template <class R, class... Args>
struct FunctionTraits<R (*)(Args...)> : FunctionTraits<R(Args...)> {
};

template <class R, class... Args>
struct FunctionTraits<R (*const)(Args...)> : FunctionTraits<R(Args...)> {
};

template <class R, class... Args>
struct FunctionTraits<R(Args...)> {
    using return_type = R;

    static constexpr std::size_t nargs = sizeof...(Args);
    static constexpr std::size_t nouts = ReturnTypeSize<R>::value;

    template <std::size_t N>
    struct argument {
        static_assert(N < nargs, "error: invalid parameter index.");
        using type = typename std::tuple_element<N, std::tuple<Args...>>::type;
    };
    using arguments = typename std::tuple<typename std::decay<Args>::type...>;
};

// static class function
template <class R, class C, class... Args>
struct FunctionTraits<R(C&&, Args&&...)> {
    using return_type = R;
    using object_type = C;

    static constexpr std::size_t nargs = sizeof...(Args);
    static constexpr std::size_t nouts = ReturnTypeSize<R>::value;

    template <std::size_t N>
    struct argument {
        static_assert(N < nargs, "error: invalid parameter index.");
        using type = typename std::tuple_element<N, std::tuple<Args...>>::type;
    };

    using arguments = typename std::tuple<typename std::decay<Args>::type...>;
};

// member function pointer
template <class C, class R, class... Args>
struct FunctionTraits<R (C::*)(Args...)> : public FunctionTraits<R(C&, Args...)> {
    using return_type = typename std::remove_reference<R>::type;
    using object_type = C;

    static constexpr std::size_t nargs = sizeof...(Args);
    static constexpr std::size_t nouts = ReturnTypeSize<R>::value;

    template <std::size_t N>
    struct argument {
        static_assert(N < nargs, "error: invalid parameter index.");
        using type = typename std::tuple_element<N, std::tuple<Args...>>::type;
    };

    using arguments = typename std::tuple<typename std::decay<Args>::type...>;
};

// const member function pointer
template <class C, class R, class... Args>
struct FunctionTraits<R (C::*)(Args...) const> : public FunctionTraits<R(C&, Args...)> {
    using return_type = R;
    using object_type = C;

    static constexpr std::size_t nargs = sizeof...(Args);
    static constexpr std::size_t nouts = ReturnTypeSize<R>::value;

    template <std::size_t N>
    struct argument {
        static_assert(N < nargs, "error: invalid parameter index.");
        using type = typename std::tuple_element<N, std::tuple<Args...>>::type;
    };

    using arguments = typename std::tuple<typename std::decay<Args>::type...>;
};

// *
template <class C, class R, class... Args>
struct FunctionTraits<R (C::*const)(Args...) const> : public FunctionTraits<R(C&, Args...)> {
    using return_type = R;
    using object_type = C;

    static constexpr std::size_t nargs = sizeof...(Args);
    static constexpr std::size_t nouts = ReturnTypeSize<R>::value;

    template <std::size_t N>
    struct argument {
        static_assert(N < nargs, "error: invalid parameter index.");
        using type = typename std::tuple_element<N, std::tuple<Args...>>::type;
    };

    using arguments = typename std::tuple<typename std::decay<Args>::type...>;
};

// member object pointer
template <class C, class R>
struct FunctionTraits<R(C::*)> : public FunctionTraits<R(C&)> {
};

template <typename... MethodTypeList>
struct MethodArgList;

template <typename... MethodTypeList>
struct MethodArgList<std::tuple<MethodTypeList...>> {
    using type = std::tuple<typename FunctionTraits<MethodTypeList>::arguments...>;
};

}

#endif // WRAPPER_FN_TRAITS_H

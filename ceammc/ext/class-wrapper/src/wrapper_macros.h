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
#ifndef WRAPPER_MACROS_H
#define WRAPPER_MACROS_H

#include "class_constructor.h"
#include "class_function.h"
#include "class_method.h"
#include "class_static_method.h"

// helper macro
#define SINGLE_ARG(...) __VA_ARGS__

/*
 * generates library entry to load via Pd object: [library]
 *
 * usage: WRAP_LIBRARY(super_library, "0.1beta", "super_library")
 */
#define WRAP_LIBRARY(name, version, pd_name)                                                       \
    wrapper_init();                                                                                \
    class lib_##name : public BaseObject {                                                         \
        SymbolProperty* name_;                                                                     \
        SymbolProperty* version_;                                                                  \
                                                                                                   \
    public:                                                                                        \
        lib_##name(const PdArgs& args)                                                             \
            : BaseObject(args)                                                                     \
            , name_(new SymbolProperty("@name", gensym(pd_name), PropValueAccess::READONLY))       \
            , version_(new SymbolProperty("@version", gensym(version), PropValueAccess::READONLY)) \
        {                                                                                          \
            addProperty(name_);                                                                 \
            addProperty(version_);                                                              \
            createOutlet();                                                                        \
        }                                                                                          \
        void onBang() override                                                                     \
        {                                                                                          \
            OBJ_DBG << "library: " << pd_name;                                                     \
        }                                                                                          \
    };                                                                                             \
    ObjectFactory<lib_##name> obj_##name(pd_name);

/*
 * generates wrapper for specified data class
 *
 * usage: WRAP_CLASS(MyData, "mydata")
 */
#define WRAP_CLASS(data_type, pd_name)                                      \
    using ctor_##data_type = wrapper::ClassConstructorCustom<data_type>;    \
    ObjectFactory<ctor_##data_type> obj_##data_type(pd_name);               \
    obj_##data_type.processData<wrapper::AbstractDataWrapper<data_type>>(); \
    obj_##data_type.addMethod("set", &ctor_##data_type::m_set)

/*
 * generates wrapper for specified method in data class
 *
 * usage: WRAP_METHOD(MyData, "mydata.size", some_uniqie_id,
 *                    SINGLE_ARG(int (MyData::*)()), SINGLE_ARG(&MyData::size))
 */
#define WRAP_METHOD(data_type, pd_name, method_id, method_arg_types, method_ptr_list)         \
                                                                                              \
    using m_base_##method_id = wrapper::ClassMethod<data_type, std::tuple<method_arg_types>>; \
    class m_##method_id : public m_base_##method_id {                                         \
                                                                                              \
    public:                                                                                   \
        m_##method_id(PdArgs& a)                                                              \
            : m_base_##method_id(a, std::make_tuple<method_arg_types>(method_ptr_list))       \
        {                                                                                     \
        }                                                                                     \
    };                                                                                        \
    ObjectFactory<m_##method_id> obj_##method_id(pd_name);                                    \
    obj_##method_id.processData<wrapper::AbstractDataWrapper<data_type>>()

/*
 * adds method alias
 *
 * usage: WRAP_METHOD_ALIAS(method_id, "alias")
 */
#define WRAP_METHOD_ALIAS(method_id, alias) obj_##method_id.addAlias(alias)

/*
 * generates wrapper for specified static method in class
 *
 * usage: WRAP_STATIC_METHOD(MyData, "mydata.from_str", method_id,
 *                           MyData (*)(const std::string&),
 *                           &Mydata::from_str)
 */
#define WRAP_STATIC_METHOD(data_type, pd_name, method_id, method_type, method_ptr)    \
                                                                                      \
    class m_##method_id : public wrapper::ClassStaticMethod<data_type, method_type> { \
                                                                                      \
    public:                                                                           \
        m_##method_id(PdArgs& a)                                                      \
            : ClassStaticMethod<data_type, method_type>(a, method_ptr)                \
        {                                                                             \
        }                                                                             \
    };                                                                                \
    ObjectFactory<m_##method_id> obj_##method_id(pd_name)

#define WRAP_FUNCTION(name, pd_name, var, type)            \
                                                           \
    class FN_##var : public wrapper::ClassFunction<type> { \
    public:                                                \
        FN_##var(PdArgs& a)                                \
            : wrapper::ClassFunction<type>(a, var)         \
        {                                                  \
        }                                                  \
    };                                                     \
    ObjectFactory<FN_##var>(pd_name)

#endif // WRAPPER_MACROS_H

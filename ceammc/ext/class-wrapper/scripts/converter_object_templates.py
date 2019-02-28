#!/usr/bin/python
import sys
import os
import re

import common_functions as gen

funcTemplate = """
// Function: {FUNCTIONNAME_CXX}
// namespace: {NAMESPACE}
using {FN_TYPE} = {FUNC_RETURN}(*)({FUNC_ARGS});
constexpr auto {FN_VAR} = &{FUNCTIONNAME_CXX};
WRAP_FUNCTION({FUNCTIONNAME_CXX}, "{FUNCTIONNAME_PD}", {FN_VAR}, {FN_TYPE});
"""

def genFunc(ns, funcNameSrc, funcReturn, funcArgs, pdname=None, aliases=[]):
    funcNamePD = pdname
    if funcNamePD is None:
        funcNamePD = gen.getClassNamePD(ns, funcNameSrc)

    funcNameCXX = gen.getClassNameCXX(ns, funcNameSrc)

    fn_type = f"ftype_{funcNameCXX}"
    fn_var = f"fvar_{funcNameCXX}"
    return funcTemplate.format(FUNCTIONNAME_CXX = funcNameCXX,
        NAMESPACE = ns,
        FUNCTIONNAME_PD = funcNamePD,
        FUNC_RETURN = funcReturn,
        FUNC_ARGS = funcArgs,
        FN_TYPE = fn_type,
        FN_VAR = fn_var)

classMethodTemplate = """
    // method: {M_PTR}
    WRAP_METHOD({CLASSNAME_CXX}, "{PD_NAME}", {M_ID},
        SINGLE_ARG({OVERLOAD_TYPES}), SINGLE_ARG({OVERLOAD_PTR}));
"""

def genClassMethod(ns,
                   className,
                   methodName,
                   methodReturn,
                   methodArgs,
                   methodConst,
                   overloadInfo=None,
                   **kwargs):

    class_name_cxx = gen.getClassNameCXX(ns, className)
    class_name_pd = gen.getClassNamePD(ns, className).replace(".", "_")
    method_name_pd = gen.convertName(methodName).replace(".", "_")

    m_id = f"m_id_{class_name_pd}_{method_name_pd}"
    m_ptr = f"&{class_name_cxx}::{methodName}"

    overload_args = [[methodArgs]] + overloadInfo
    overload_args = list(map(
        lambda t: "{}({}::*)({}) {}".format(
            methodReturn, class_name_cxx, ",".join(t), "const" if methodConst else ""), overload_args))

    overload_ptr = ", ".join([m_ptr] * len(overload_args))

    res = classMethodTemplate.format(
        CLASSNAME_CXX = class_name_cxx,
        M_ID = m_id,
        M_PTR = m_ptr,
        PD_NAME = gen.method_pd_name(ns, class_name_pd, method_name_pd, **kwargs),
        OVERLOAD_TYPES = ", ".join(overload_args),
        OVERLOAD_PTR = overload_ptr)

    for alias in kwargs.get("pd_aliases", []):
        pd_alias = f"{alias}"
        res += f"    WRAP_METHOD_ALIAS({m_id}, \"{pd_alias}\");\n"

    return res

classStaticMethodTemplate = """
    // static method
    using {M_TYPE} = {STATIC_METHOD_RETURN}(*)({STATIC_METHOD_ARGS});
    WRAP_STATIC_METHOD({CLASSNAME_CXX}, "{PD_NAME}", {M_ID}, {M_TYPE}, {M_PTR});
"""

def genClassStaticMethod(ns,
                         className,
                         methodName,
                         methodReturn,
                         methodArgs,
                         **kwargs):
    class_name_cxx = gen.getClassNameCXX(ns, className)
    class_name_pd = gen.getClassNamePD(ns, className).replace(".","_")
    method_name_pd = gen.convertName(methodName).replace(".", "_")

    m_type = f"mtype_{class_name_pd}_{method_name_pd}"
    m_ptr = f"&{class_name_cxx}::{methodName}"
    m_id = f"m_id_{class_name_pd}_{method_name_pd}"

    res = classStaticMethodTemplate.format(ns = ns,
        CLASSNAME_CXX = class_name_cxx,
        M_TYPE = m_type,
        M_ID = m_id,
        M_PTR = m_ptr,
        STATIC_METHOD_RETURN = methodReturn,
        STATIC_METHOD_ARGS = methodArgs,
        PD_NAME = gen.method_pd_name(ns, class_name_pd, method_name_pd, **kwargs))

    for alias in kwargs.get("pd_aliases", []):
        pd_alias = f"{alias}"
        res += f"    WRAP_METHOD_ALIAS({m_id}, \"{pd_alias}\");\n"

    return res

classTemplate = """
    // class: {CLASSNAME_CXX}
    WRAP_CLASS({CLASSNAME_CXX}, "{CLASSNAME_PD}.new");
"""

def genClass(ns, name):
    """
    generates code for class wrapper
    ns - ns
    name - class name
    """
    name_cxx = gen.getClassNameCXX(ns, name)
    name_pd = gen.getClassNamePD(ns, name)

    return classTemplate.format(
        CLASSNAME_CXX = name_cxx,
        CLASSNAME_PD = name_pd,
        ns = ns)

libEntryTemplate = """
    // library: {NAME}
    WRAP_LIBRARY({NAME}, "{VERSION}", "{NAME}");
"""

def genLibraryEntry(name, version):
    return libEntryTemplate.format(NAME=name, VERSION=version)

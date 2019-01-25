#!/usr/bin/python

import sys, os, re
from termcolor import colored, cprint

def error_msg(msg):
    print("{0} {1}".format(colored("[Error]", "red"), msg))

def warning_msg(msg):
    print("{0} {1}".format(colored("[Warning]", "yellow"), msg))

def debug_msg(msg):
    print("{0}".format(colored(msg, "blue")))

def method_pd_name(nameSpaceSrc, classNameSrc, methodNameSrc, **kwargs):
    classNamePD = getClassNamePD(nameSpaceSrc, classNameSrc)
    methodNamePD = convertName(methodNameSrc)

    if "pd_fullname" in kwargs:
        return kwargs["pd_fullname"]
    else:
        ext_ns = kwargs.get("pd_ns", nameSpaceSrc)
        ext_class = kwargs.get("pd_class", classNamePD)
        ext_name = kwargs.get("pd_name", methodNamePD)

        res = f"{ext_class}.{ext_name}"
        if ext_ns:
            res = f"{ext_ns}.{external_name}"

        return res

# camelCase -> camel-case
def convertNameWithH(name):
    s1 = re.sub('(.)([A-Z][a-z]+)', r'\1-\2', name)
    return re.sub('([a-z0-9])([A-Z])', r'\1-\2', s1).lower()

# camelCase -> camelcase
def convertName(name):
    s1 = re.sub('(.)([A-Z][a-z]+)', r'\1\2', name)
    return re.sub('([a-z0-9])([A-Z])', r'\1\2', s1).lower()

def getHeaderFiles():
    ret = []
    try:
        ret = ret + os.listdir("../../to_wrap/")
    except:
        ret = os.listdir("../to_wrap/")

    return ret;

# TODO: test this
def getHeaderFilesFullPath(path):
    ret = []

    files = os.listdir(path)
    for filename in files:
        if filename.endswith(".hpp") or filename.endswith(".h"):
            ret += [path + filename]

    return ret;

#####
# def getPdObjectName(cxxClassName):
#     return "object"
#
# def getPdObjectName(cxxClassName, cxxMethodName):
#     return "object.method"
#####

def getClassNameCXX(nameSpace, classNameSrc):
    nameSpaceDivider = "::"
    if (nameSpace == ""):
        nameSpaceDivider = ""
    if len(nameSpace) > 0:
        ret = nameSpace + "::" + classNameSrc
    else:
        ret = classNameSrc
    return ret

def getClassNamePD(nameSpace, classNameSrc):
    ret = convertName(getClassNameCXX(nameSpace, classNameSrc))
    ret = ret.replace("::", ".")
    return ret

#!/usr/bin/python
import sys
import os
import re

_dir_path = os.path.dirname(os.path.realpath(__file__))

from jinja2 import Environment, FileSystemLoader, select_autoescape
env = Environment(
    loader = FileSystemLoader(_dir_path),
    autoescape=select_autoescape(['pddoc', 'xml']),
    trim_blocks=True,
    lstrip_blocks=True
)

#TODO: template XML files

import common_functions as gen
import tempfile

global __outputDir__
__outputDir__ = ""

global __libraryConfig__

def escapeTypeString(typeString):
    ret = typeString.replace("<","(")
    ret = ret.replace(">",")")
    ret = ret.replace("&","(reference)")
    ret = ret.replace("*","(pointer)")
    return ret

def classObject(library, **kwargs):
    template = env.get_template('class_template.pddoc')

    global __outputDir__

    txt = template.render(
        ALIASES = kwargs.get("pd_aliases", []),
        ALSO = kwargs.get("see", []),
        ARGS = kwargs["pddoc"].get("arguments", []),
        AUTHORS = kwargs.get("authors", []),
        CATEGORY = kwargs["pd_class"],
        CLASSNAME_PD = kwargs["pd_class"],
        DATATYPE = kwargs["class"],
        DESCR = kwargs.get("brief", ""),
        EXAMPLE = kwargs["pddoc"].get("example", ""),
        INLETS = kwargs["pddoc"].get("inlets", []),
        KEYWORDS = " ".join(kwargs.get("keywords", [])),
        LIBRARY = library,
        LICENSE = kwargs.get("license", "???"),
        OBJECTNAME_PD = kwargs["pd_class"] + ".new",
        SINCE = kwargs.get("since", ""),
        VERSION = kwargs.get("version", "0.0")
    )

    fileName = "{}/{}.new.pddoc".format(__outputDir__, kwargs["pd_class"])

    if os.path.exists(fileName):
        with open(fileName, 'r') as fp:
            data = fp.read()
            if txt == data:
                return list()

    outputFile = open(fileName, "w+")
    outputFile.write(txt)
    return [fileName]

def classMethodObject(library, **kwargs):
    template = env.get_template('class_method_template.pddoc')

    global __outputDir__

    txt = template.render(
        LIBRARY = library, \
        CATEGORY = kwargs["doc_class"]["pd_name"], \
        VERSION = kwargs.get("version", "0.0"), \
        LICENSE = kwargs.get("license", "???"), \
        AUTHORS = kwargs.get("authors", []), \
        KEYWORDS = " ".join(kwargs.get("keywords", [])), \
        ALIASES = kwargs.get("pd_aliases", []), \
        ALSO = kwargs.get("see", []), \
        SINCE = kwargs.get("since", ""), \
        DESCR = kwargs.get("brief", ""), \
        OBJECTNAME_PD = kwargs["doc_name"], \
        CLASSNAME_PD = kwargs["doc_class"]["pd_name"],\
        EXAMPLE = kwargs.get("example", ""),\
        EXAMPLE_CLASS_ARGS = kwargs["doc_class"].get("example_args", []),\
        ARGS = kwargs.get("pddoc_args", []), \
        INLETS = kwargs.get("doc_inlets", []), \
        NOUTLETS = len(kwargs.get("doc_outlets", 0)), \
        OUTLET_INFO = [inlet.get("info", "") for inlet in kwargs["doc_outlets"]], \
        OUTLET_TYPE = [inlet.get("type", "") for inlet in kwargs["doc_outlets"]])

    fileName = __outputDir__ + "/" + kwargs["doc_name"].replace("/", "\\/") + ".pddoc"

    if os.path.exists(fileName):
        with open(fileName, 'r') as fp:
            data = fp.read()
            if txt == data:
                return list()

    outputFile = open(fileName, "w+")
    outputFile.write(txt)

    return [fileName]

def staticMethodObject(className,description,objectType, methodType,methodReturn, cppClassName):
    global __outputDir__
    global __libraryConfig__

    fileName = __outputDir__+objectType+".pddoc"
    outputFile = open(fileName,"w+")

    methodTypeStr = ",".join(methodType)
    methodTypeStr = escapeTypeString(methodTypeStr)

    methodReturnStr = escapeTypeString(str(methodReturn))

    if methodType ==  []:
        methodTypeStr = "accepts bang"
    else:
        methodTypeStr = "accepts list: {0}".format(methodTypeStr)

    if methodReturn == []:
        methodReturn = "no output"
    else:
        methodReturn = "list: {0}".format(methodReturn)

    outputFile.write("""<?xml version="1.0" encoding="utf-8"?>
<pddoc version="1.0">
    <object name="{OBJECTNAME_PD}">
        <title>{OBJECTNAME_PD}</title>
        <info>
            <par>{DESCR}</par>
        </info>
        <meta>
            <authors>
                <author>{CFG_AUTHORS}</author>
            </authors>
            <description>Static method of {CLASSNAME_CXX}</description>
            <license>{CFG_LICENSE}</license>
            <library>{CFG_LIBRARY}</library>
            <category>{CLASSNAME_PD}</category>
            <keywords>none</keywords>
            <since>1.0</since>
        </meta>
        <inlets>
            <inlet>
                <xinfo on="any">{METHOD_ARGS_STR} </xinfo>
                <xinfo on="symbol">'thread 1' sets the object to perform it's action in separate thread</xinfo>
                <xinfo on="symbol">'func' outputs object's function as DataAtom at second outlet</xinfo>
            </inlet>
        </inlets>
        <outlets>
            <outlet>{METHOD_RETURN_STR}</outlet>
            <outlet>Outputs DataAtom with {OBJECTNAME_PD} function.</outlet>
        </outlets>
        <example>
            <pdascii>
<![CDATA[
[declare -lib {CFG_LIBRARY}]

[B] [1.0( [symbol test( [1 2 3(
|   |     |             |
[{OBJECTNAME_PD}                               ]
|
[ui.display @display_type 1]

[loadbang]
|
[thread 1(
|
|  [B] [1.0( [symbol test( [1 2 3(
|  |   |     |             |
[{OBJECTNAME_PD}                               ]
|
[ui.display @display_type 1]

[func(
|
[{OBJECTNAME_PD}]
^|
[ui.display @display_type 1]

]]>
            </pdascii>
        </example>
    </object>
</pddoc>
    """.format(CLASSNAME_PD = className, \
    DESCR = description, \
    OBJECTNAME_PD = objectType, \
    METHOD_ARGS_STR = methodTypeStr, \
    METHOD_RETURN_STR = methodReturnStr,
    CLASSNAME_CXX =cppClassName, \
    CFG_AUTHORS = __libraryConfig__["authors"], \
    CFG_LICENSE = __libraryConfig__["license"], \
    CFG_LIBRARY = __libraryConfig__["library_name"]))
# ----------

def customClassObject(className,description,objectType, infoString):
    global __outputDir__
    global __libraryConfig__

    fileName = __outputDir__+objectType+".pddoc"
    outputFile = open(fileName,"w+")
    outputFile.write("""<?xml version="1.0" encoding="utf-8"?>
<pddoc version="1.0">
    <object name="{CLASSNAME_PD}">
        <title>{CLASSNAME_PD}</title>
        <info>
            <par>{DESCR}</par>
        </info>
        <meta>
            <authors>
                <author>{CFG_AUTHORS}</author>
            </authors>
            <description>Creates new instances of {CLASSNAME_CXX}</description>
            <license>{CFG_LICENSE}</license>
            <library>{CFG_LIBRARY}</library>
            <category>{CLASSNAME_CXX}</category>
            <keywords> none </keywords>
            <since> 1.0 </since>
        </meta>
        <inlets>
            <inlet>
                <xinfo on="bang">Outputs current instance of {CLASSNAME_CXX} </xinfo>
                <xinfo on="any">Creates new instance of {CLASSNAME_CXX} and outputs it</xinfo>
            </inlet>
        </inlets>
        <outlets>
            <outlet>Outputs DataAtom with {CLASSNAME_CXX} object.</outlet>
        </outlets>
        <example>
            <pdascii>
<![CDATA[
[declare -lib {CFG_LIBRARY}]

[B] [1.0( [symbol test( [1 2 3(
|   |     |             |
[{CLASSNAME_PD}                        ]
|
[ui.display @display_type 1]

]]>
            </pdascii>
        </example>
    </object>
</pddoc>
    """.format(CLASSNAME_CXX = className, \
    DESCR = description, \
    CLASSNAME_PD = objectType, \
    INFO_STRING = infoString, \
    CFG_AUTHORS = __libraryConfig__["authors"], \
    CFG_LICENSE = __libraryConfig__["license"], \
    CFG_LIBRARY = __libraryConfig__["library_name"]))

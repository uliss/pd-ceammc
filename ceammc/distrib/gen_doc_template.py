#!/usr/bin/env python3

# -*- coding: utf-8 -*-
from lxml import etree, objectify
import sys
import argparse
import subprocess
import os.path
from lxml import etree, objectify
from termcolor import colored, cprint
import json

SRC_PATH = "@PROJECT_SOURCE_DIR@/"
BIN_PATH = "@PROJECT_BINARY_DIR@/ceammc/ext/src/lib/"
DOC_PATH = "@PROJECT_SOURCE_DIR@/ceammc/ext/doc/"
STK_RAWWAVES_PATH = "@PROJECT_SOURCE_DIR@/ceammc/extra/stk/stk/rawwaves"
VERSION="@CEAMMC_LIB_VERSION@"

EXT_LIST = BIN_PATH + "ext_list"
EXT_INFO = BIN_PATH + "ext_info"

IGNORE_METHODS = ["dsp"]


def read_all_externals():
    return list(filter(lambda x: len(x), subprocess.check_output([EXT_LIST], stderr=subprocess.DEVNULL).decode().split('\n')))


def read_info(name, *args):
    try:
        args = [EXT_INFO, name]
        s = subprocess.check_output(args, stderr=subprocess.DEVNULL, env={
                                    "RAWWAVES": STK_RAWWAVES_PATH}).decode()
        return json.loads(s)
    except(subprocess.CalledProcessError) as e:
        if e.returncode != 4:
            cprint(f"[{name}] can't get properties", "red")

        return None


def dict2xml(name, dic):
    """
    Returns etree object with xml from dict
    Expects dicts that can be transformed to the following structure:
    <key>value</key> or <key>dict</key>
    i.e. something like dict = {str:str|dict}
    """
    def __subnodes(inner_dic):
        subnodes = []
        for k, v in inner_dic.items():
            elem = etree.Element(k)
            if isinstance(v, dict):
                for subnode in __subnodes(v):
                    elem.append(subnode)
            else:
                elem.text = v.__str__()

            subnodes.append(elem)
        return subnodes

    try:
        root = etree.Element(name)
        for node in __subnodes(dic):
            root.append(node)
    except Exception as e:
        raise
    return root


def create_object(root, name):
    obj = objectify.Element("object")
    obj.set("name", name)
    obj.title = name

    obj.append(
        dict2xml("meta", {
            "authors": {"author": "Serge Poltavski"},
            "description": " ",
            "license": "GPL3 or later",
            "library": "ceammc",
            "category": name.split('.')[0],
            "keywords": "",
            "since": VERSION,
            "also": {"see": " "}
        })
    )

    obj.append(
        dict2xml("arguments", {
            "argument": " "
        })
    )

    obj.append(
        dict2xml("properties", {
            "property": " "
        })
    )

    root.append(obj)


def create_xml():
    xml = '''<?xml version="1.0" encoding="utf-8"?>
    <pddoc></pddoc>
    '''

    root = objectify.fromstring(xml.encode())
    root.set("version", "1.0")
    return root


if __name__ == '__main__':
    parser = argparse.ArgumentParser(
        description='CEAMMC Pd pddoc template generator')
    parser.add_argument('-v', '--verbose',
                        help='verbose output', action='store_true')
    parser.add_argument('-p', '--stdout',
                        help='ouput to stdout', action='store_true')
    parser.add_argument('-f', '--force',
                        help='force overrite of existing file', action='store_true')
    parser.add_argument('external', metavar='EXT_NAME',
                        type=str, help='external name')

    args = parser.parse_args()
    ext_name = args.external
    ext_list = read_all_externals()

    if ext_name not in ext_list:
        cprint(
            f"Error: [{ext_name}] is not found in CEAMMC external list", "red")
        sys.exit(1)

    info = read_info(ext_name)
    if info is None:
        sys.exit(3)

    if args.verbose:
        cprint(
            f" - generating template for [{ext_name}] external ...", "green")

    if not args.stdout:
        pddoc_path = f"{DOC_PATH}{ext_name}.pddoc"
        if not args.force and os.path.exists(pddoc_path):
            cprint(f"Error: {pddoc_path} already exists ...", "red")
            sys.exit(2)

        if args.verbose:
            cprint(f" - pddoc file: \"{pddoc_path}\"", "blue")

    xml = create_xml()

    create_object(xml, info["object"]["name"])

    obj = xml["object"]

    # description
    obj.meta.description = " "
    if "info" in info["object"] and "description" in info["object"]["info"]:
        obj.meta.description = info["object"]["info"]["description"]

    # category
    obj.meta.category = " "
    if "info" in info["object"] and "category" in info["object"]["info"]:
        obj.meta.category = info["object"]["info"]["category"]

    # aliases
    if "info" in info["object"] and "aliases" in info["object"]["info"]:
        obj.meta.aliases = ""
        for a in info["object"]["info"]["aliases"]:
            obj.meta.aliases.alias = f"{a}"

    # keywords
    if "info" in info["object"] and "keywords" in info["object"]["info"]:
        obj.meta.keywords = " ".join(info["object"]["info"]["keywords"])

    # create properties
    obj.properties = ""
    if "properties" in info["object"]:
        for p in info["object"]["properties"]:
            m = etree.Element("property")
            m.set("name", p["name"])
            m.set("type", p["type"])
            if "units" in p:
                m.set("units", p["units"])
            if "default" in p:
                m.set("default", p["default"].__str__())
            if "min" in p:
                m.set("minvalue", p["min"].__str__())
            if "max" in p:
                m.set("maxvalue", p["max"].__str__())
            if "enum" in p:
                m.set("enum", " ".join(p["enum"]))
            m.text = "..."
            obj.properties.append(m)

    # create methods
    obj.methods = ""
    if "methods" in info["object"]:
        nm = len(info["object"]["methods"])
        if nm > 0:
            for i in range(nm):
                mname = info["object"]["methods"][i]
                if mname in IGNORE_METHODS:
                    continue

                m = etree.SubElement(obj.methods, "method")
                m.set("name", mname)
                m._setText("...")

    # create inlets
    ninl = len(info["object"]["inlets"])
    obj.inlets = ""
    if ninl > 0:
        for i in range(ninl):
            obj.inlets.inlet = ""
            obj.inlets.inlet.set("type", info["object"]["inlets"][i])
            obj.inlets.inlet.xinfo = f"inlet {i}"

    # create outlets
    nout = len(info["object"]["outlets"])
    obj.outlets = ""
    if nout > 0:
        for i in range(nout):
            obj.outlets.outlet = f"outlet {i}"
            obj.outlets.outlet.set("type", info["object"]["inlets"][i])

    # create example
    obj.example = ""
    pdascii = etree.Element("pdascii")
    pdascii.text = etree.CDATA(f'\n[bang(\n|\n[{ext_name}]\n')
    obj.example.append(pdascii)

    objectify.deannotate(xml)
    etree.cleanup_namespaces(xml)

    try:
        xml_str = etree.tostring(
            xml, pretty_print=True, xml_declaration=True, encoding='utf-8').decode("utf-8")
        if args.verbose:
            print(xml_str)

        if args.stdout:
            sys.stdout.write(
                etree.tostring(
                    xml,
                    pretty_print=True,
                    xml_declaration=True,
                    encoding='utf-8').decode('utf-8'))
        else:
            with open(pddoc_path, "wb") as xml_writer:
                xml_writer.write(
                    etree.tostring(
                        xml,
                        pretty_print=True,
                        xml_declaration=True,
                        encoding='utf-8'))

    except IOError:
        pass

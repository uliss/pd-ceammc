#!/usr/bin/env python3

import sys
import signal

def signal_handler(sig, frame):
    sys.exit(0)

signal.signal(signal.SIGINT, signal_handler)

import argparse
import subprocess
import os.path
from lxml import etree
from termcolor import colored, cprint
import json
import jamspell

BIN_PATH = "@PROJECT_BINARY_DIR@/ceammc/ext/src/lib/"
DOC_PATH = "@PROJECT_SOURCE_DIR@/ceammc/ext/doc/"

EXT_LIST = BIN_PATH + "ext_list"
EXT_METHODS = BIN_PATH + "ext_methods"
EXT_PROPS = BIN_PATH + "ext_props"

SPECIAL_OBJ = {"function": "f"}

corrector = jamspell.TSpellCorrector()

def read_all_externals():
    return list(filter(lambda x: len(x), subprocess.check_output([EXT_LIST], stderr=subprocess.DEVNULL).decode().split('\n')))

def read_methods(name):
    # methods starting with @ - properties in UI objects
    # methods starting with . - internal methods
    # methods ending with _aliased - overwritten methods
    def valid_method(x):
        return (len(x) and x[0] != '@' and x[0] != '.') and (not str(x).endswith("_aliased"))

    try:
        args = [EXT_METHODS, name]
        if name in SPECIAL_OBJ:
            args.append(SPECIAL_OBJ[name])

        return set(filter(valid_method,
            subprocess.check_output(args, stderr=subprocess.DEVNULL).decode().split('\n')))
    except(subprocess.CalledProcessError):
        cprint(f"[{name}] can't get methods", "red")
        return set()

def read_props(name):
    try:
        s = subprocess.check_output([EXT_PROPS, name], stderr=subprocess.DEVNULL).decode()
        js = json.loads(s)
        return set(js.keys()), js
    except(subprocess.CalledProcessError):
        cprint(f"[{name}] can't get properties", "red")
        return set(), dict()

def check_spell(obj):
    if obj.text:
        txt = obj.text.strip()
        if len(txt) and obj.tag not in ('object', 'title', 'alias'):
            upd = corrector.FixFragment(txt)
            if txt != upd:
                print("spell errors found:")
                cprint(f"\t{txt}", "red")
                cprint(f"\t{upd}\n", "green")

    for node in obj:
        # skip CDATA
        if node.tag == 'pdascii':
            continue

        check_spell(node)

if __name__ == '__main__':
    parser = argparse.ArgumentParser(description='CEAMMC Pd documentation checker')
    parser.add_argument('-a', '--all', help='check all', action='store_true')
    parser.add_argument('-m', '--methods', help='check methods', action='store_true')
    parser.add_argument('-p', '--props', help='check properties', action='store_true')
    parser.add_argument('-s', '--spell', help='check spell', action='store_true')
    parser.add_argument('-v', '--verbose', help='verbose output', action='store_true')
    parser.add_argument('external', metavar='EXT_NAME', type=str, help='external name')

    args = parser.parse_args()
    ext_name = args.external

    if args.all:
        args.props = True
        args.methods = True
        args.spell = True

    ext_list = read_all_externals()
    if ext_name not in ext_list:
        print(f"Error: [{ext_name}] is not found in CEAMMC external list")
        sys.exit(1)

    if args.verbose:
        print(f" - checking [{ext_name}] external ...")

    pddoc_path = f"{DOC_PATH}{ext_name}.pddoc"
    if not os.path.exists(pddoc_path):
        print(f"Error: {pddoc_path} is not found ...")
        sys.exit(2)

    if args.verbose:
        print(f" - pddoc file: \"{pddoc_path}\"")

    with open(pddoc_path) as fobj:
        xml = fobj.read()

    root = etree.fromstring(xml.encode())

    doc_methods_set = set()
    doc_props_set = set()
    doc_props_dict = dict()

    # parse methods
    for appt in root:
        for x in appt:
            if x.tag == "methods":
                for m in x:
                    if m.tag != "method" or m.get("example", False) or m.get("internal", False):
                        continue
                    doc_methods_set.add(m.attrib["name"].split(' ')[0])

            if x.tag == "properties":
                for p in x:
                    if p.tag != "property":
                        continue

                    name = p.attrib["name"]
                    doc_props_set.add(name)
                    doc_props_dict[name] = dict()
                    if "units" in p.attrib:
                        doc_props_dict[name]["units"] = p.attrib["units"]
                    if "minvalue" in p.attrib:
                        doc_props_dict[name]["minvalue"] = p.attrib["minvalue"]
                    if "maxvalue" in p.attrib:
                        doc_props_dict[name]["maxvalue"] = p.attrib["maxvalue"]
                    if "default" in p.attrib:
                        doc_props_dict[name]["default"] = p.attrib["default"]
                    if "enum" in p.attrib:
                        doc_props_dict[name]["enum"] = p.attrib["enum"]
                    if "readonly" in p.attrib:
                        doc_props_dict[name]["readonly"] = p.attrib["readonly"]

                    doc_props_dict[name]["type"] = p.attrib["type"]

    if args.methods:
        ext_methods = read_methods(ext_name)
        # print(doc_methods_set)
        # print(ext_methods)
        ignored_methods = {'dump', 'dsp', 'signal', 'mouseup', 'mouseenter', 'dialog', 'iscicm',
        'zoom', 'mousewheel', 'mousemove', 'mousedown', 'mouseleave',
        'symbol', 'float', 'bang', 'dblclick', 'list', 'dsp_add', 'loadbang',
        'click', 'dsp_add_aliased', 'vis', 'popup', 'eobjreadfrom', 'eobjwriteto'}
        undoc_methods_set = ext_methods - doc_methods_set - ignored_methods
        unknown_methods = doc_methods_set - ext_methods
        if len(undoc_methods_set):
            cprint(f"[{ext_name}] undocumented methods: {undoc_methods_set}", 'magenta')

        if len(unknown_methods):
            cprint(f"[{ext_name}] unknown methods in doc: {unknown_methods}", 'yellow')

    if args.props:
        ignored_props = {'@*'}

        ext_props_set, ext_props_dict = read_props(ext_name)
        undoc_props_set = ext_props_set - doc_props_set - ignored_props
        unknown_props = doc_props_set - ext_props_set
        exists_props = ext_props_set & doc_props_set

        if len(undoc_props_set):
            cprint(f"[{ext_name}] undocumented properties: {undoc_props_set}", 'magenta')

        if len(unknown_props):
            cprint(f"[{ext_name}] unknown properties in doc: {unknown_props}", 'yellow')

        HAVE_PDDOC = -1
        HAVE_EXTERNAL = 1
        HAVE_NONE = 0
        HAVE_BOTH = 2

        def check_attr(name, p0, p1):
            a0 = name in p0
            a1 = name in p1

            if a0 == a1:
                if a0:
                    return HAVE_BOTH
                else:
                    return HAVE_NONE
            else:
                if a0:
                    return HAVE_EXTERNAL
                else:
                    return HAVE_PDDOC

        for p in exists_props:
            p0 = ext_props_dict[p]
            p1 = doc_props_dict[p]

            # readonly in external
            if p0.get("readonly", False):
                # but not in doc
                if not p1.get("readonly", False):
                    cprint(f"[{ext_name}] missing readonly attribute in \"{p}\"", 'magenta')

            # readonly in docs
            if "readonly" in p1 and p1["readonly"] == "true":
                # but not readonly in external
                if not p0.get("readonly", False):
                    cprint(f"[{ext_name}] non-readonly attribute in \"{p}\"", 'red')

            # units checks
            if p1.get("units", False):
                if not p0.get("units", False):
                    cprint(f"[{ext_name}] missing units attribute in external \"{p}\"", 'magenta')
                elif p1["units"] != p0["units"]:
                    u0 = p0["units"]
                    u1 = p1["units"]
                    cprint(f"[{ext_name}] non-equal units in \"{p}\": {u0} != {u1}", 'magenta')

            if p0.get("units", False):
                if not p1.get("units", False):
                    cprint(f"[{ext_name}] missing units attribute in pddoc \"{p}\"", 'magenta')

            if p0["type"] == "bool":
                if p1["type"] == "flag":
                    continue

                if "enum" not in p1:
                    cprint(f"[{ext_name}] missing attribute enum for bool in \"{p}\"", 'magenta')

                # no default bool value
                if ("default" in p0) and ("readonly" not in p0) and ("default" not in p1):
                    cprint(f"[{ext_name}] missing attribute default for bool in \"{p}\"", 'magenta')

                # non-equal default bool values
                if "default" in p0 and "default" in p1:
                    v0 = str(p0["default"])
                    v1 = str(p1["default"])
                    if v0 != v1:
                        cprint(f"[{ext_name}] invalid value for default attribute \"{p}\": {v0} != {v1}", 'magenta')

                continue

            if p0["type"] != p1["type"]:
                if p0["type"] == "symbol":
                    if p1["type"] not in ("symbol", "alias"):
                        cprint(f"[{ext_name}] invalid symbol type in \"{p}\"", 'magenta')
                    else:
                        pass
                elif p0["type"] == "bool" and p1["type"] != "int":
                    cprint(f"[{ext_name}] invalid bool type (not int) in \"{p}\"", 'magenta')
                else:
                    t0 = p0["type"]
                    t1 = p1["type"]
                    cprint(f"[{ext_name}] different attr types: {t0} != {t1} in \"{p}\"", 'magenta')

            if "min" in p0 and "minvalue" not in p1:
                cprint(f"[{ext_name}] missing attribute minvalue in \"{p}\"", 'magenta')

            if "max" in p0 and "maxvalue" not in p1:
                cprint(f"[{ext_name}] missing attribute maxvalue in \"{p}\"", 'magenta')

            if ("default" in p0) and ("readonly" not in p0) and ("default" not in p1):
                cprint(f"[{ext_name}] missing attribute default in \"{p}\"", 'magenta')

            if "default" in p0 and "default" in p1:
                v0 = str(p0["default"])
                v1 = str(p1["default"])
                if v0 != v1:
                    cprint(f"[{ext_name}] invalid value for default attribute \"{p}\": {v0} != {v1}", 'magenta')

            if "min" in p0 and "minvalue" in p1:
                v0 = str(p0["min"])
                v1 = str(p1["minvalue"])
                if v0 != v1:
                    cprint(f"[{ext_name}] invalid value for minvalue attribute \"{p}\": {v0} != {v1}", 'magenta')

            if "max" in p0 and "maxvalue" in p1:
                v0 = str(p0["max"])
                v1 = str(p1["maxvalue"])
                if v0 != v1:
                    cprint(f"[{ext_name}] invalid value for maxvalue attribute \"{p}\": {v0} != {v1}", 'magenta')

            attr = check_attr("enum", p0, p1)
            if attr == HAVE_BOTH:
                v0 = set(p0["enum"])
                v1 = set(p1["enum"].split(" "))

                if v0 != v1:
                    cprint(f"[{ext_name}] invalid value for enum attribute \"{p}\": {v0} != {v1}", 'magenta')
            elif attr == HAVE_EXTERNAL:
                cprint(f"[{ext_name}] missing enum attribute in pddoc \"{p}\"", 'magenta')
            elif attr == HAVE_PDDOC:
                    cprint(f"[{ext_name}] pddoc enum for attribute \"{p}\" not exists", 'magenta')


    if args.spell:
        corrector.LoadLangModel('ceammc.bin')
        cprint(f"checking [{ext_name}] ...", "blue")
        check_spell(root)

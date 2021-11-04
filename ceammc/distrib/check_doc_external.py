#!/usr/bin/env python3

import sys
import signal
import argparse
import subprocess
import os.path
from lxml import etree
from termcolor import cprint
import json

SRC_PATH = "@PROJECT_SOURCE_DIR@/"
BIN_PATH = "@PROJECT_BINARY_DIR@/ceammc/ext/src/lib/"
DOC_PATH = "@PROJECT_SOURCE_DIR@/ceammc/ext/doc/"
STK_RAWWAVES_PATH = "@PROJECT_SOURCE_DIR@/ceammc/extra/stk/stk/rawwaves"

EXT_LIST = BIN_PATH + "ext_list"
EXT_METHODS = BIN_PATH + "ext_methods"
EXT_PROPS = BIN_PATH + "ext_props"

SPECIAL_OBJ = {"function": "f"}


def signal_handler(sig, frame):
    sys.exit(0)


signal.signal(signal.SIGINT, signal_handler)

def read_all_externals():
    return list(filter(lambda x: len(x), subprocess.check_output([EXT_LIST], stderr=subprocess.DEVNULL).decode().split('\n')))


def read_methods(name):
    # methods starting with @ - properties in UI objects
    # methods starting with . - internal methods
    # methods ending with _aliased - overwritten methods
    def valid_method(x):
        return (len(x) and x[0] != '@') and (not str(x).endswith("_aliased"))

    try:
        args = [EXT_METHODS, name]
        if name in SPECIAL_OBJ:
            args.append(SPECIAL_OBJ[name])

        return set(filter(valid_method,
            subprocess.check_output(args, stderr=subprocess.DEVNULL,
                                    env={"RAWWAVES": STK_RAWWAVES_PATH}).decode().split('\n')))
    except(subprocess.CalledProcessError):
        cprint(f"[{name}] can't get methods", "red")
        return set()


def read_props(name):
    try:
        args = [EXT_PROPS, name]
        if name in SPECIAL_OBJ:
            args.append(SPECIAL_OBJ[name])

        s = subprocess.check_output(args, stderr=subprocess.DEVNULL,
                                    env={"RAWWAVES": STK_RAWWAVES_PATH},
                                    encoding="utf-8", errors="ignore")
        js = json.loads(s)
        return set(js.keys()), js
    except(subprocess.CalledProcessError) as e:
        if e.returncode != 4:
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
    parser.add_argument('-p', '--props', help='check properties', action='store_true')
    parser.add_argument('-s', '--spell', help='check spell', action='store_true')
    parser.add_argument('-v', '--verbose', help='verbose output', action='store_true')
    parser.add_argument('external', metavar='EXT_NAME', type=str, help='external name')

    args = parser.parse_args()
    ext_name = args.external

    if args.all:
        args.props = True
        args.spell = True

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

    ext_methods = read_methods(ext_name)
    is_ceammc = '.is_cicm?' in ext_methods or '.is_base?' in ext_methods
    ext_methods = set([x for x in ext_methods if len(x) and x[0] != '.' and x[0] != '_'])
    # print(doc_methods_set)
    # print(ext_methods)
    ignored_methods = {'dump', 'dsp', 'signal', 'mouseup', 'mouseenter', 'dialog',
                       'zoom', 'mousewheel', 'mousemove', 'mousedown', 'mouseleave',
                       'symbol', 'float', 'bang', 'dblclick', 'list', 'dsp_add', 'loadbang',
                       'click', 'dsp_add_aliased', 'vis', 'popup', 'eobjreadfrom', 'eobjwriteto',
                       'rightclick', 'key' }
    undoc_methods_set = ext_methods - doc_methods_set - ignored_methods
    unknown_methods = doc_methods_set - ext_methods
    if len(undoc_methods_set):
        cprint(f"[{ext_name}] undocumented methods: {undoc_methods_set}", 'magenta')

    if len(unknown_methods):
        cprint(f"[{ext_name}] unknown methods in doc: {unknown_methods}", 'yellow')

    if args.props and is_ceammc:
        ignored_props = {'@*', '@label', '@label_margins', '@label_valign', '@label_align', '@label_inner', '@label_side', '@label_color'}

        ext_props_set, ext_props_dict = read_props(ext_name)
        undoc_props_set = ext_props_set - doc_props_set - ignored_props
        unknown_props = doc_props_set - ext_props_set
        exists_props = ext_props_set & doc_props_set

        undoc_props_set = {x for x in undoc_props_set if ext_props_dict[x].get("visibility", "") != "internal" }

        if len(undoc_props_set):
            cprint(f"[{ext_name}] undocumented properties: {undoc_props_set}", 'magenta')

        if len(unknown_props):
            cprint(f"[{ext_name}] unknown properties in doc: {unknown_props}", 'yellow')

        # check internal props
        internal_props = {x for x in ext_props_set if ext_props_dict[x].get("visibility", "") == "internal" }
        for p in internal_props:
            if p in doc_props_dict:
                cprint(f"[{ext_name}] internal property in doc: {p}", 'magenta')

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
            if p0.get("access", "") == "readonly":
                # but not in doc
                if "readonly" not in p1 and p1.get("readonly", "") != "true":
                    cprint(f"DOC [{ext_name}] missing readonly attribute in \"{p}\"", 'magenta')

            # readonly in docs
            if "readonly" in p1 and p1["readonly"]:
                # but not readonly in external
                if p0.get("access", "") not in ("readonly", "initonly"):
                    cprint(f"EXT [{ext_name}] non-readonly attribute in \"{p}\"", 'red')

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
                    cprint(f"DOC [{ext_name}] missing units \"{p}\"", 'magenta')

            if p0["type"] == "bool":
                if p1["type"] in ("flag", "alias"):
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

            # aliases
            if p0["type"] == "int" and p1["type"] in ("flag", "alias"):
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

            if "min" in p0 and "minvalue" in p1:
                v0 = str(p0["min"])
                v1 = str(p1["minvalue"])
                if v0 != v1:
                    cprint(f"DOC [{ext_name}] invalid minvalue \"{p}\": {v1}, in external: {v0}", 'magenta')

            if "max" in p0 and "maxvalue" in p1:
                v0 = str(p0["max"])
                v1 = str(p1["maxvalue"])
                if v1 == "2π":
                    v1 = "6.28319"

                if v0 != v1:
                    cprint(f"[{ext_name}] invalid value for maxvalue attribute \"{p}\": {v0} != {v1}", 'magenta')

            attr = check_attr("default", p0, p1)
            if attr == HAVE_BOTH:
                if isinstance(p0["default"], list):
                    v0 = p0["default"]
                    v1 = p1["default"].split(" ")

                    if len(v0) > 0 and (isinstance(v0[0], float) or isinstance(v0[0], int)):
                        v1 = list(map(float, v1))
                else:
                    v0 = str(p0["default"])
                    v1 = p1["default"]

                if v1 == "+inf" and float(v0) > 1.0e+24:
                    pass
                elif v1 == "-inf" and float(v0) < -1.0e+24:
                    pass
                elif v0 == "Set " and v1 == "Set()":
                    pass
                elif v0 != v1:
                    cprint(f"DOC [{ext_name}] invalid default \"{p}\": {v1}, in external: {v0}", 'magenta')
            elif attr == HAVE_EXTERNAL:
                vdef = p0["default"]
                is_empty = hasattr(vdef, '__len__') and len(vdef) == 0
                if not is_empty:
                    cprint(f"EXT [{ext_name}] missing default in doc \"{p}\"", 'magenta')

            attr = check_attr("enum", p0, p1)
            if attr == HAVE_BOTH:
                v0 = set(p0["enum"])
                v1 = set(p1["enum"].split(" "))

                if len(p0["enum"]) > 0:
                    e1 = []
                    for x in p1["enum"].split(" "):
                        try:
                            if x == "inf":
                                raise 1

                            y = float(x)
                            if y.is_integer():
                                y = int(x)

                            e1.append(y)
                        except:
                            e1.append(x)

                    v1 = set(e1)

                if v0 != v1:
                    cprint(f"[{ext_name}] invalid value for enum attribute \"{p}\": {v0} != {v1}", 'magenta')
                    d0 = v0 - v1
                    d1 = v1 - v0
                    if len(d0):
                        cprint(f"[{ext_name}] missing elements are: {d0}", 'magenta')
                    if len(d1):
                        cprint(f"[{ext_name}] invalid elements in doc are: {d1}", 'magenta')

            elif attr == HAVE_EXTERNAL:
                if ext_name.startswith("ui.") and p not in ("@fontname"):
                    cprint(f"DOC [{ext_name}] missing enum attribute\"{p}\"", 'magenta')
            elif attr == HAVE_PDDOC:
                    cprint(f"DOC [{ext_name}] no enum for attribute \"{p}\" (in external)", 'magenta')

    if args.spell:
        import jamspell
        corrector = jamspell.TSpellCorrector()

        corrector.LoadLangModel('ceammc.bin')
        cprint(f"checking [{ext_name}] ...", "blue")
        check_spell(root)

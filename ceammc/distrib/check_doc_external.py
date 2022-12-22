#!/usr/bin/env python3

import sys
import signal
import argparse
import subprocess
import os.path
import time
from lxml import etree
from termcolor import cprint
import json

verbose_output = False

SRC_PATH = "@PROJECT_SOURCE_DIR@/"
BIN_PATH = "@PROJECT_BINARY_DIR@/ceammc/ext/src/lib/"
DOC_PATH = "@PROJECT_SOURCE_DIR@/ceammc/ext/doc/"
STK_RAWWAVES_PATH = "@PROJECT_SOURCE_DIR@/ceammc/extra/stk/stk/rawwaves"

EXT_INFO = BIN_PATH + "ext_info"

SPECIAL_OBJ = {"function": "f"}

EXT_INLETS = []
EXT_OUTLETS = []
EXT_METHODS = set()
EXT_ALIASES = set()
EXT_PROPS_SET = set()
EXT_PROPS_DICT = dict()

def signal_handler(sig, frame):
    sys.exit(0)


signal.signal(signal.SIGINT, signal_handler)


# methods starting with @ - properties in UI objects
# methods starting with . - internal methods
# methods ending with _aliased - overwritten methods
def is_valid_method(x):
    return (len(x) and x[0] != '@') and (not str(x).endswith("_aliased"))

def read_ext_info(name):
    global EXT_INLETS, EXT_OUTLETS, EXT_METHODS, EXT_ALIASES, EXT_PROPS_SET, EXT_PROPS_DICT

    try:
        args = [EXT_INFO, name]
        if name in SPECIAL_OBJ:
            args.append(SPECIAL_OBJ[name])

        output = subprocess.check_output(args,
                                        env={"RAWWAVES": STK_RAWWAVES_PATH},
                                        stderr=subprocess.DEVNULL).decode()

        js = json.loads(output)
        # read xlets
        try:
            EXT_INLETS = js["object"]["inlets"]
            EXT_OUTLETS = js["object"]["outlets"]
        except(KeyError):
            pass
        # read methods
        try:
            EXT_METHODS = set(filter(is_valid_method, js["object"]["methods"]))
        except(KeyError):
            pass
        # read aliases
        try:
            EXT_ALIASES = set(js["object"]["info"]["aliases"])
        except(KeyError):
            pass

        # read props
        try:
            props = js["object"]["properties"]
            for p in props:
                EXT_PROPS_SET.add(p["name"])
                EXT_PROPS_DICT[p["name"]] = p
        except(KeyError):
            pass

        return True
    except(subprocess.CalledProcessError):
        cprint(f"[{name}] can't get methods", "red")
        return False


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


def read_doc_aliases(node):
    res = set()

    if node.tag != "meta":
        return res

    for x in node:
        if x.tag == "aliases":
            for y in x:
                if y.tag == "alias":
                    res.add(y.text.strip())

    return res


def read_doc_inlets(node):
    res = []

    if node.tag != "inlets":
        return res

    if node.attrib.get("dynamic", "false") == "true":
        return None

    for x in node:
        if x.tag == "inlet":
            res.append(x.attrib.get("type", "control"))

    return res


def read_doc_outlets(node):
    res = []

    if node.tag != "outlets":
        return res

    if node.attrib.get("dynamic", "false") == "true":
        return None

    for x in node:
        if x.tag == "outlet":
            res.append(x.attrib.get("type", "control"))

    return res

def check_aliases(name, doc, ext):
    undoc_aliases = ext - doc
    unknown_aliases = doc - ext

    if len(undoc_aliases) > 0:
        cprint(f"[{ext_name}] undocumented aliases: {undoc_aliases}",
            'magenta')

    if len(unknown_aliases) > 0:
        cprint(f"[{ext_name}] unknown aliases: {unknown_aliases}",
            'yellow')


def check_methods(name, doc, ext):
    ignored_methods = {'dump', 'dsp', 'signal', 'mouseup', 'mouseenter', 'dialog',
                       'zoom', 'mousewheel', 'mousemove', 'mousedown', 'mouseleave',
                       'symbol', 'float', 'bang', 'dblclick', 'list', 'dsp_add', 'loadbang',
                       'click', 'dsp_add_aliased', 'vis', 'popup', 'eobjreadfrom', 'eobjwriteto',
                       'rightclick', 'key' }

    undoc_methods_set = ext - doc - ignored_methods
    unknown_methods = doc - ext
    if len(undoc_methods_set):
        cprint(f"[{ext_name}] undocumented methods: {undoc_methods_set}",
            'magenta')

    if len(unknown_methods):
        cprint(f"[{ext_name}] unknown methods in doc: {unknown_methods}",
            'yellow')


def check_xlets(name, doc_in, doc_out, ext_in, ext_out):
    if doc_in is not None and doc_in != ext_in:
        cprint(f"[{ext_name}] inlets error: {doc_in} != {ext_in}",
            'magenta')

    if doc_out is not None and doc_out != ext_out:
        cprint(f"[{ext_name}] outlets error: {doc_out} != {ext_out}",
            'magenta')


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description='CEAMMC Pd documentation checker')
    parser.add_argument('-a', '--all', help='check all', action='store_true')
    parser.add_argument('-p', '--props', help='check properties', action='store_true')
    parser.add_argument('-m', '--methods', help='check methods', action='store_true')
    parser.add_argument('-i', '--aliases', help='check aliases', action='store_true')
    parser.add_argument('-s', '--spell', help='check spell', action='store_true')
    parser.add_argument('-x', '--xlets', help='check inlets/outlets', action='store_true')
    parser.add_argument('-v', '--verbose', help='verbose output', action='store_true')
    parser.add_argument('external', metavar='EXT_NAME', type=str, help='external name')

    args = parser.parse_args()
    ext_name = args.external
    verbose_output = args.verbose

    if args.all:
        args.props = True
        args.spell = True
        args.aliases = True
        args.methods = True
        args.xlets = True

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
    doc_aliases = set()
    doc_inlets = None
    doc_outlets = None

    read_ext_info(ext_name)

    # parse PDDOC info
    for appt in root:
        for x in appt:
            # parse pddoc aliases
            if args.aliases:
                a = read_doc_aliases(x)
                if len(a) > 0:
                    doc_aliases = a

            if args.xlets:
                z = read_doc_inlets(x)
                if z is not None and len(z) > 0:
                    doc_inlets = z

                z = read_doc_outlets(x)
                if z is not None and len(z) > 0:
                    doc_outlets = z

            if args.methods:
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

    if args.xlets:
        check_xlets(ext_name, doc_inlets, doc_outlets, EXT_INLETS, EXT_OUTLETS)

    if args.aliases:
        check_aliases(ext_name, doc_aliases, EXT_ALIASES)

    if args.methods:
        check_methods(ext_name, doc_methods_set, EXT_METHODS)

    if args.props:
        ignored_props = {'@*', '@label', '@label_margins', '@label_valign',
            '@label_align',
            '@label_inner',
            '@label_side',
            '@label_color'
            }

        ext_props_set = EXT_PROPS_SET
        ext_props_dict = EXT_PROPS_DICT
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
                v0 = str(round(p0["min"], 3))
                v1 = str(p1["minvalue"])
                if v0 != v1:
                    cprint(f"DOC [{ext_name}] invalid minvalue \"{p}\": {v1}, in external: {v0}", 'magenta')

            if "max" in p0 and "maxvalue" in p1:
                v0 = str(round(p0["max"], 3))
                v1 = str(p1["maxvalue"])
                if v1 == "2π":
                    v1 = "6.28319"

                if v0 != v1:
                    cprint(f"[{ext_name}] invalid value for maxvalue attribute \"{p}\": {v0} != {v1}", 'magenta')

            attr = check_attr("default", p0, p1)
            if attr == HAVE_BOTH:
                v0 = p0["default"]
                v1 = p1["default"]
                if isinstance(v0, list):
                    v0 = "(" + " ".join(v0) + ")"
                    v1 = p1["default"].split(" ")

                    if len(v0) > 0 and (isinstance(v0[0], float) or isinstance(v0[0], int)):
                        v1 = list(map(float, v1))
                elif isinstance(v0, dict):
                    v0 = "[]"
                else:
                    if isinstance(v0, float):
                        v0 = round(v0, 3)

                    v0 = str(v0)

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
        try:
            import jamspell
            corrector = jamspell.TSpellCorrector()

            corrector.LoadLangModel('ceammc.bin')
            cprint(f"checking [{ext_name}] ...", "blue")
            check_spell(root)
        except ModuleNotFoundError as e:
            cprint(f"{e} ...", "red")
            pass

#!/usr/bin/env python3

import sys
import signal
import argparse
import subprocess
import os.path
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
EXT_ARGS_DICT = dict()

def signal_handler(sig, frame):
    sys.exit(0)


signal.signal(signal.SIGINT, signal_handler)


# methods starting with @ - properties in UI objects
# methods starting with . or _ - internal methods
# methods ending with _aliased - overwritten methods
def is_valid_method(x):
    return (len(x) and x[0] not in ('@', '_')) and (not str(x).endswith("_aliased"))

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
                if p is None:
                    EXT_PROPS_DICT[p["name"]] = "null"
        except(KeyError):
            pass

        # read args
        try:
            args = js["object"]["args"]
            for a in args:
                EXT_ARGS_DICT[a["name"]] = a
                if a is None:
                    EXT_ARGS_DICT[a["name"]] = "null"
        except(KeyError):
            pass

        return True
    except(subprocess.CalledProcessError):
        cprint(f"[{name}] can't get external info", "red")
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

def has_mouse_methods():
    return ("click" in EXT_METHODS or
        "mousedown" in EXT_METHODS or
        "mouseup" in EXT_METHODS or
        "mousemove" in EXT_METHODS or
        "mouseenter" in EXT_METHODS or
        "rightclick" in EXT_METHODS)


def check_units(name, tag, doc, ext):
    if tag[0] == '@':
        tag = f'[{tag}]'
    else:
        tag = f'[arg][{tag}]'


    def unit2pddoc(unit: str):
        MAP = { 'millisecond': 'msec', 'second': 'sec' }
        if unit in MAP:
            return MAP[unit]
        else:
            return unit

    units_doc = set()
    doc_unit_str = doc.get("units", None)
    if doc_unit_str is not None:
        for x in doc_unit_str.split(" "):
            units_doc.add(unit2pddoc(x))

    units_ext = set()
    for x in ext.get("units", ()):
        units_ext.add(unit2pddoc(x))

    if units_doc != units_ext:
        cprint(f"[{name}]{tag} units in doc {units_doc} != units in ext {units_ext}", 'red')
        miss = units_ext - units_doc
        if len(miss) > 0:
            cprint(f"[{name}]{tag} missing units: {miss}", 'red')

        invalid = units_doc - units_ext
        if len(invalid) > 0:
            cprint(f"[{name}]{tag} invalid units: {invalid}", 'red')


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
                       'onzoom', 'zoom', 'mousewheel', 'mousemove', 'mousedown', 'mouseleave',
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


def check_single_arg(ext_name, arg_name, doc, ext):
    # check type
    doc_type = doc.get("type", "")
    ext_type = ext.get("type", "")

    if doc_type != ext_type:
        cprint(f"[{ext_name}][arg][{arg_name}] invalid argument type in doc: {doc_type}, should be: {ext_type}", 'magenta')

    # check minvalue
    doc_minval = doc.get("min", "")
    ext_minval = ext.get("min", "")
    if ext_type == "float":
        if doc_minval != "":
            doc_minval = round(float(doc_minval), 4)
        if ext_minval != "":
            ext_minval = round(float(ext_minval), 4)

    if doc_minval != ext_minval:
        cprint(f"[{ext_name}][arg][{arg_name}] invalid argument minvalue in doc: {doc_minval}, should be: {ext_minval}", 'magenta')

    # check maxvalue
    doc_maxval = doc.get("max", "")
    ext_maxval = ext.get("max", "")
    if ext_type == "float":
        if doc_maxval != "":
            doc_maxval = round(float(doc_maxval), 4)
        if ext_maxval != "":
            ext_maxval = round(float(ext_maxval), 4)
    if doc_maxval != ext_maxval:
        cprint(f"[{ext_name}][arg][{arg_name}] invalid argument maxvalue in doc: {doc_maxval}, should be: {ext_maxval}", 'magenta')

    check_units(ext_name, arg_name, doc, ext)


def check_args(name, doc, ext):
    doc_args_set = set(doc.keys())
    ext_args_set = set(ext.keys())

    undoc_args = ext_args_set - doc_args_set
    unknown_args = doc_args_set - ext_args_set
    if len(undoc_args):
        cprint(f"[{ext_name}] undocumented arguments: {undoc_args}",
            'magenta')

    if len(unknown_args):
        cprint(f"[{ext_name}] unknown arguments in doc: {unknown_args}",
            'yellow')

    for a in doc_args_set.intersection(ext_args_set):
        check_single_arg(name, a, doc[a], ext[a])

def check_xlets(name, doc_in, doc_out, ext_in, ext_out):
    if doc_in is not None and doc_in != ext_in:
        cprint(f"[{ext_name}] inlets error: {doc_in} != {ext_in}",
            'magenta')

    if doc_out is not None and doc_out != ext_out:
        cprint(f"[{ext_name}] outlets error: {doc_out} != {ext_out}",
            'magenta')


def check_single_prop(name, prop, doc, ext):
    # access check
    ro_ext = ext.get("access", "")
    ro_doc = doc.get("access", "")
    if ro_ext != ro_doc:
        cprint(f"[{ext_name}][{prop}] access in doc ({ro_doc}) != access in ext ({ro_ext})", 'red')

    check_units(name, prop, doc, ext)

    # check minvalue
    doc_minval = doc.get("min", "")
    if doc_minval == "π":
        doc_minval = round(3.1415926, 4)
    elif doc_minval == "2π":
        doc_minval = round(3.1415926 * 2, 4)
    elif len(doc_minval) > 0:
        doc_minval = float(doc_minval)

    ext_minval = ext.get("min", "")
    if isinstance(ext_minval, str) and len(ext_minval) > 0:
        ext_minval = round(float(ext_minval), 4)
    elif isinstance(ext_minval, float):
        ext_minval = round(ext_minval, 4)

    if doc_minval != ext_minval:
        cprint(f"[{ext_name}][{prop}] invalid property minvalue in doc: {doc_minval}, should be: {ext_minval}", 'magenta')

    # check maxvalue
    doc_maxval = doc.get("max", "")
    if doc_maxval == "π":
        doc_maxval = round(3.1415926, 4)
    elif doc_maxval == "2π":
        doc_maxval = round(3.1415926 * 2, 4)
    elif len(doc_maxval) > 0:
        doc_maxval = float(doc_maxval)

    ext_maxval = ext.get("max", "")
    if isinstance(ext_maxval, str) and len(ext_maxval) > 0:
        ext_maxval = round(float(ext_maxval), 4)
    elif isinstance(ext_maxval, float):
        ext_maxval = round(ext_maxval, 4)

    if doc_maxval != ext_maxval:
        cprint(f"[{ext_name}][{prop}] invalid property maxvalue in doc: {doc_maxval}, should be: {ext_maxval}", 'magenta')

    type_doc = doc.get("type", None)
    type_ext = ext.get("type", None)

    if type_ext == "bool" and ro_ext == "initonly": # flag type
        pass
    else:
        # check default
        doc_def = doc.get("default", None)
        # none
        if doc_def is None:
            pass
        # float
        elif type_doc == "float":
            if doc_def == "2π":
                doc_def = round(3.1415926 * 2, 4)
            elif doc_def == "π":
                doc_def = round(3.1415926, 4)
            elif doc_def == "+inf":
                doc_def = 2147483647
            elif doc_def == "-inf":
                doc_def = -2147483648
            else:
                doc_def = round(float(doc_def), 4)
        # int
        elif type_doc == "int":
            doc_def = int(doc_def)
        elif type_doc == "bool":
            doc_def = int(doc_def)

        ext_def = ext.get("default", None)
        if ext_def is None:
            pass
        elif type_ext == "float":
            ext_def = round(ext_def, 4)
        elif type_ext == "int":
            ext_def = round(ext_def, 4)
        elif type_ext == "list":
            def conv(x):
                if isinstance(x, (int, float)):
                    return str(round(x, 4))
                elif x is None:
                    return ""
                else:
                    return x

            try:
                ext_def = " ".join(map(lambda x: conv(x), ext_def))
            except TypeError as e:
                cprint(f"[{ext_name}][{prop}] exception while checking {e}", 'red')

        if type_ext == "atom":
            doc_def = str(doc_def)
            ext_def = str(ext_def)

        if doc_def != ext_def:
            cprint(f"[{ext_name}][{prop}] invalid property default in doc: {doc_def}, should be: {ext_def}", 'magenta')

        # end default check

    # check enums
    doc_enum = doc.get("enum", set())
    ext_enum = set(ext.get("enum", set()))
    if isinstance(doc_enum, str):
        doc_enum = set(doc_enum.split(" "))
        if type_doc == "float":
            doc_enum = set(map(lambda x: float(x), doc_enum))
        elif type_doc == "int":
            doc_enum = set(map(lambda x: int(x), doc_enum))
        elif type_doc == "atom":
            def get_atom(x):
                if isinstance(x, str) and len(x) > 0 and x[0].isdigit():
                    return float(x)
                else:
                    return x

            doc_enum = set(map(lambda x: get_atom(x), doc_enum))

    if doc_enum != ext_enum:
        cprint(f"[{ext_name}][{prop}] invalid property enum in doc: {doc_enum}, should be: {ext_enum}", 'magenta')
        doc_miss = ext_enum - doc_enum
        if len(doc_miss) > 0:
            cprint(f"\t- missing {doc_miss}", 'yellow')
            x = " ".join(map(str, sorted(list(doc_miss))))
            cprint(f"\t  add to doc: enum=\"{x}\"", 'white')

        doc_invalid = doc_enum - ext_enum
        if len(doc_invalid) > 0:
            cprint(f"\t- invalid {doc_invalid}", 'yellow')

    # check types
    type_doc = doc.get("type", None)
    type_ext = ext.get("type", None)

    if type_doc != type_ext:
        if type_ext == "bool" and type_doc == "int":
            cprint(f"[{ext_name}][{prop}] fix bool type property", 'magenta')
        elif type_ext == "bool" and ro_ext == "initonly":
            if type_doc != "flag":
                cprint(f"[{ext_name}][{prop}] should be in doc as flag property", 'magenta')
        elif type_doc == "alias":
            pass
        else:
            cprint(f"[{ext_name}][{prop}] type in doc ({type_doc}) != type in ext ({type_ext})", 'magenta')


def check_props(name, doc, ext):
    ignored_props = {'@*',
        '@label',
        '@label_margins',
        '@label_valign',
        '@label_align',
        '@label_inner',
        '@label_side',
        '@label_color'
        }

    doc_props_set = set(doc.keys())
    ext_props_set = set(ext.keys())

    undoc_props = { x for x in (ext_props_set - doc_props_set - ignored_props) if ext[x].get("visibility", "") != "internal" }
    unknown_props = doc_props_set - ext_props_set

    if len(undoc_props):
        cprint(f"[{ext_name}] undocumented properties: {undoc_props}", 'magenta')

    if len(unknown_props):
        cprint(f"[{ext_name}] unknown properties in doc: {unknown_props}", 'yellow')

    # check internal props
    internal_props = { x for x in ext_props_set if ext[x].get("visibility", "") == "internal" }
    for p in internal_props:
        if p in doc:
            cprint(f"[{ext_name}] internal property in doc: {p}", 'magenta')

    for prop in doc_props_set & ext_props_set:
        check_single_prop(name, prop, doc[prop], ext[prop])



if __name__ == '__main__':
    parser = argparse.ArgumentParser(description='CEAMMC Pd documentation checker')
    parser.add_argument('-a', '--all', help='check all', action='store_true')
    parser.add_argument('-p', '--props', help='check properties', action='store_true')
    parser.add_argument('-m', '--methods', help='check methods', action='store_true')
    parser.add_argument('-i', '--aliases', help='check aliases', action='store_true')
    parser.add_argument('-s', '--spell', help='check spell', action='store_true')
    parser.add_argument('-x', '--xlets', help='check inlets/outlets', action='store_true')
    parser.add_argument('-g', '--args', help='check arguments', action='store_true')
    parser.add_argument('-v', '--verbose', help='verbose output', action='store_true')
    parser.add_argument('external', metavar='EXT_NAME', type=str, help='external name')

    args = parser.parse_args()
    ext_name = args.external
    verbose_output = args.verbose

    if args.all:
        args.props = True
        # args.spell = True
        args.aliases = True
        args.methods = True
        args.xlets = True
        args.args = True

    pddoc_path = f"{DOC_PATH}{ext_name}.pddoc"
    if not os.path.exists(pddoc_path):
        print(f"Error: {pddoc_path} is not found ...")
        sys.exit(2)

    if args.verbose:
        print(f" - pddoc file: \"{pddoc_path}\"")

    with open(pddoc_path) as fobj:
        xml = fobj.read()

    cprint(f"checking [{ext_name}] ...", "blue")

    doc = etree.parse(pddoc_path)
    doc.xinclude()
    root = doc.getroot()

    doc_methods_set = set()
    doc_props_set = set()
    doc_props_dict = dict()
    doc_args_dict = dict()
    doc_aliases = set()
    doc_inlets = None
    doc_outlets = None
    doc_mouse = set()

    if not read_ext_info(ext_name):
        sys.exit(1)

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

            if x.tag == "mouse":
                if not has_mouse_methods():
                    cprint(f"[{ext_name}] unknown mouse events in doc", 'yellow')
                else:
                    for event in x:
                        if "type" in event.attrib:
                            doc_mouse.add(event.attrib["type"])

            if args.args and x.tag == "arguments":
                for a in x:
                    if a.tag != "argument":
                        continue

                    name = a.attrib["name"]
                    doc_args_dict[name] = dict()
                    if "units" in a.attrib:
                        doc_args_dict[name]["units"] = a.attrib["units"]
                    if "minvalue" in a.attrib:
                        doc_args_dict[name]["min"] = float(a.attrib["minvalue"])
                    if "maxvalue" in a.attrib:
                        doc_args_dict[name]["max"] = float(a.attrib["maxvalue"])
                    if "enum" in a.attrib:
                        doc_args_dict[name]["enum"] = a.attrib["enum"]

                    doc_args_dict[name]["type"] = a.attrib["type"]

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
                        doc_props_dict[name]["min"] = p.attrib["minvalue"]
                    if "maxvalue" in p.attrib:
                        doc_props_dict[name]["max"] = p.attrib["maxvalue"]
                    if "default" in p.attrib:
                        doc_props_dict[name]["default"] = p.attrib["default"]
                    if "enum" in p.attrib:
                        doc_props_dict[name]["enum"] = p.attrib["enum"]

                    doc_props_dict[name]["access"] = p.attrib.get("access", "readwrite")
                    doc_props_dict[name]["type"] = p.attrib["type"]

    if args.xlets:
        check_xlets(ext_name, doc_inlets, doc_outlets, EXT_INLETS, EXT_OUTLETS)

    if args.aliases:
        check_aliases(ext_name, doc_aliases, EXT_ALIASES)

    if args.methods:
        check_methods(ext_name, doc_methods_set, EXT_METHODS)

    if args.args:
        check_args(ext_name, doc_args_dict, EXT_ARGS_DICT)

    if args.props:
        check_props(ext_name, doc_props_dict, EXT_PROPS_DICT)

    if "click" in EXT_METHODS and len(doc_mouse) == 0:
        cprint(f"[{ext_name}] no mouse event documentation", 'magenta')

    if False:
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

            attr = check_attr("default", p0, p1)
            if attr == HAVE_BOTH:
                v0 = p0["default"]
                v1 = p1["default"]
                if isinstance(v0, list):
                    v1 = p1["default"].split(" ")

                    if len(v0) > 0 and (isinstance(v0[0], float) or isinstance(v0[0], int)):
                        v1 = list(map(float, v1))
                        v0 = list(map(lambda x: round(x, 3), v0))
                    elif len(v0) == 0:
                        v0 = "()"
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
            check_spell(root)
        except ModuleNotFoundError as e:
            cprint(f"{e} ...", "red")
            pass

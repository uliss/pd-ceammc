#!/usr/bin/env python3

import argparse
import subprocess
import os.path
from lxml import etree
from termcolor import colored, cprint
import json

SRC_PATH = "@PROJECT_SOURCE_DIR@/"
BIN_PATH = "@PROJECT_BINARY_DIR@/ceammc/ext/src/lib/"
DOC_PATH = "@PROJECT_SOURCE_DIR@/ceammc/ext/doc/"
STK_RAWWAVES_PATH = "@PROJECT_SOURCE_DIR@/ceammc/extra/stk/stk/rawwaves"

EXT_LIST = BIN_PATH + "ext_list"
EXT_METHODS = BIN_PATH + "ext_methods"
EXT_PROPS = BIN_PATH + "ext_props"


def read_all_externals():
    return list(filter(lambda x: len(x), subprocess.check_output([EXT_LIST], stderr=subprocess.DEVNULL).decode().split('\n')))

def read_props(name):
    try:
        args = [EXT_PROPS, name]
        s = subprocess.check_output(args, stderr=subprocess.DEVNULL, env={"RAWWAVES": STK_RAWWAVES_PATH}).decode()
        js = json.loads(s)
        return set(js.keys()), js
    except(subprocess.CalledProcessError) as e:
        if e.returncode != 4:
            cprint(f"[{name}] can't get properties", "red")

        return set(), dict()


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description='Update pddoc properties default values')
    parser.add_argument('-v', '--verbose', help='verbose output', action='store_true')
    parser.add_argument('-p', '--pretend', help='just see changes, do not save', action='store_true')
    parser.add_argument('-d', '--default', help='update default attributes', action='store_true')
    parser.add_argument('-u', '--units', help='update units attributes', action='store_true')
    parser.add_argument('-l', '--limits', help='update limit attributes (min/max)', action='store_true')
    parser.add_argument('-g', '--generate', help='generate doc templates', action='store_true')
    parser.add_argument('-a', '--all', help='update all supported attributes', action='store_true')
    parser.add_argument('external', metavar='EXT_NAME', type=str, help='external name')

    args = parser.parse_args()
    ext_name = args.external

    if args.all:
        args.default = True
        args.units = True
        args.limits = True

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

    parser = etree.XMLParser(strip_cdata=False)
    root = etree.XML(xml.encode(), parser)

    doc_props_set = set()
    doc_props_dict = dict()

    doc_props_nodes = dict()
    xml_props_node = None

    # read external properties
    for appt in root:
        for x in appt:
            if x.tag == "properties":
                xml_props_node = x
                for p in x:
                    if p.tag != "property":
                        continue

                    name = p.attrib["name"]
                    doc_props_nodes[name] = p
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

                    doc_props_dict[name]["type"] = p.attrib["type"]


    ignored_props = {'@*', '@label', '@label_margins', '@label_valign', '@label_align', '@label_inner', '@label_side', '@label_color'}

    ext_props_set, ext_props_dict = read_props(ext_name)
    exists_props = ext_props_set & doc_props_set

    if args.generate:
        for p in ext_props_set:
            if not p in doc_props_dict:
                p_ext = ext_props_dict[p]

                cprint(f"[{ext_name}] generate template entry for \"{p}\"", 'magenta')
                x = etree.SubElement(xml_props_node, "property")
                x.attrib["name"] = p
                x.attrib["type"] = p_ext["type"]
                if "min" in p_ext:
                    x.attrib["minvalue"] = str(p_ext["min"])
                if "max" in p_ext:
                    x.attrib["maxvalue"] = str(p_ext["max"])
                if "default" in p_ext:
                    x.attrib["default"] = str(p_ext["default"])
                if "units" in p_ext:
                    x.attrib["units"] = str(p_ext["units"])

                x.text = "...."


    for p in exists_props:
        p_ext = ext_props_dict[p]
        p_doc = doc_props_dict[p]

        if args.default and "default" in p_ext:
            vdef = p_ext["default"]

            if isinstance(vdef, list):
                vdef = " ".join(str(x) for x in vdef)
            else:
                vdef = str(vdef)

            if vdef == "":
                continue

            if "default" not in p_doc:
                cprint(f"[{ext_name}] setting default to {vdef} for \"{p}\"", 'magenta')
                doc_props_nodes[p].attrib["default"] = vdef
            elif p_doc["default"] != vdef:
                cprint(f"[{ext_name}] update default to {vdef} for \"{p}\"", 'magenta')
                doc_props_nodes[p].attrib["default"] = vdef

        if args.limits and "min" in p_ext:
            vmin = str(p_ext["min"])

            if "minvalue" not in p_doc:
                cprint(f"[{ext_name}] setting minvalue to {vmin} for \"{p}\"", 'magenta')
                doc_props_nodes[p].attrib["minvalue"] = vmin
            elif p_doc["minvalue"] != vmin:
                cprint(f"[{ext_name}] update minvalue to {vmin} for \"{p}\"", 'magenta')
                doc_props_nodes[p].attrib["minvalue"] = vmin

        if args.limits and "max" in p_ext:
            vmax = str(p_ext["max"])

            if "maxvalue" not in p_doc:
                cprint(f"[{ext_name}] setting maxvalue to {vmax} for \"{p}\"", 'magenta')
                doc_props_nodes[p].attrib["maxvalue"] = vmax
            elif p_doc["minvalue"] != vmin:
                cprint(f"[{ext_name}] update maxvalue to {vmax} for \"{p}\"", 'magenta')
                doc_props_nodes[p].attrib["maxvalue"] = vmax

        if args.units and "units" in p_ext:
            vunits = str(p_ext["units"])

            if "units" not in p_doc:
                cprint(f"[{ext_name}] setting units to {vunits} for \"{p}\"", 'magenta')
                doc_props_nodes[p].attrib["units"] = vunits
            elif p_doc["units"] != vunits:
                cprint(f"[{ext_name}] update units to {vunits} for \"{p}\"", 'magenta')
                doc_props_nodes[p].attrib["units"] = vunits


    if args.verbose:
        print(etree.tostring(root, pretty_print=True).decode("utf-8"))

    if not args.pretend:
        with open(pddoc_path, 'wb') as f:
            f.write(etree.tostring(root, pretty_print=True, doctype='<?xml version="1.0" encoding="utf-8"?>'))



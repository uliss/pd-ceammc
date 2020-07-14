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
from jsonpath_ng import jsonpath, parse

SRC_PATH = "@PROJECT_SOURCE_DIR@/"
BIN_PATH = "@PROJECT_BINARY_DIR@/ceammc/ext/src/lib/"
DOC_PATH = "@PROJECT_SOURCE_DIR@/ceammc/ext/doc/"
STK_RAWWAVES_PATH = "@PROJECT_SOURCE_DIR@/ceammc/extra/stk/stk/rawwaves"
VERSION="@CEAMMC_LIB_VERSION@"

EXT_LIST = BIN_PATH + "ext_list"
EXT_INFO = BIN_PATH + "ext_info"

SPECIAL_OBJ = {"function": "f"}

def read_all_externals():
    return list(filter(lambda x: len(x), subprocess.check_output([EXT_LIST], stderr=subprocess.DEVNULL).decode().split('\n')))


def read_info(name, *args):
    try:
        args = [EXT_INFO, name]
        if name in SPECIAL_OBJ:
            args.append(SPECIAL_OBJ[name])

        s = subprocess.check_output(args, stderr=subprocess.DEVNULL, env={
                                    "RAWWAVES": STK_RAWWAVES_PATH}, encoding="utf-8")
        return json.loads(s)
    except(subprocess.CalledProcessError) as e:
        if e.returncode != 4:
            cprint(f"[{name}] can't get properties", "red")

        return None


if __name__ == '__main__':
    parser = argparse.ArgumentParser(
        description='CEAMMC Pd pddoc update utility')
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
        print(f" - checking [{ext_name}] external ...")

    pddoc_path = f"{DOC_PATH}{ext_name}.pddoc"
    if not os.path.exists(pddoc_path):
        print(f"Error: {pddoc_path} is not found ...")
        sys.exit(2)

    with open(pddoc_path) as fobj:
        xml = fobj.read()

    parser = etree.XMLParser(strip_cdata=False)
    root = etree.XML(xml.encode(), parser)

    upd_counter = 0

    # description
    for match in parse("object.info.description").find(info):
        new_descr = match.value
        if len(new_descr) < 1:
            break

        el = root.xpath("//object/meta/description")
        if len(el) == 1 and el[0].text != new_descr:
            cprint(f"updating description", "green")
            print(f"\t{new_descr}")
            el[0].text = new_descr
            upd_counter += 1

    # category
    for match in parse("object.info.category").find(info):
        new_cat = match.value
        if len(new_cat) < 1:
            break

        el = root.xpath("//object/meta/category")
        if len(el) == 1 and el[0].text != new_cat:
            cprint(f"updating category", "green")
            print(f"\t{new_cat}")
            el[0].text = new_cat
            upd_counter += 1

    # keywords
    kw = parse("object.info.keywords[*]").find(info)
    if len(kw) > 0:
        el = root.xpath("//object/meta/keywords")
        if len(el) == 0:
            root.xpath("//object/meta")[0].append(etree.Element("keywords"))
            el = root.xpath("//object/meta/keywords")

        new_kw = " ".join(map(lambda x: x.value, kw))
        if new_kw != el[0].text:
            el[0].text = new_kw
            cprint(f"updating keywords", "green")
            print(f"\t{new_kw}")
            upd_counter += 1

    # aliases
    aliases = parse("object.info.aliases[*]").find(info)
    if len(aliases) > 0:
        el = root.xpath("//object/meta/aliases")
        if len(el) == 0:
            root.xpath("//object/meta")[0].append(etree.Element("aliases"))
            el = root.xpath("//object/meta/aliases")

        src_al = set()
        for xel in el[0]:
            src_al.add(xel.text.strip())

        dest_al = set()
        for a in aliases:
            dest_al.add(a.value)

        del_al = src_al - dest_al
        add_al = dest_al - src_al

        if len(del_al) > 0:
            cprint(f"removing aliases:", "red")
            print(f"\t{del_al}")

        if len(add_al) > 0:
            cprint(f"adding aliases:", "green")
            print(f"\t{add_al}")

        if len(del_al) > 0 or len(add_al) > 0:
            upd_counter += 1
            el[0].clear()
            for a in aliases:
                na = etree.Element("alias")
                na.text = a.value
                el[0].append(na)

    # authors
    authors = parse("object.info.authors[*]").find(info)
    if len(authors) > 0:
        el = root.xpath("//object/meta/authors")
        if len(el) == 0:
            root.xpath("//object/meta")[0].append(etree.Element("authors"))
            el = root.xpath("//object/meta/authors")

        src_a = set()
        for xel in el[0]:
            src_a.add(xel.text.strip())

        dest_a = set()
        for a in authors:
            dest_a.add(a.value)

        del_a = src_a - dest_a
        add_a = dest_a - src_a

        if len(del_a) > 0:
            cprint(f"removing authors:", "red")
            print(f"\t{del_a}")

        if len(add_a) > 0:
            cprint(f"adding authors:", "green")
            print(f"\t{add_a}")

        if len(del_a) > 0 or len(add_a) > 0:
            upd_counter += 1
            el[0].clear()
            for a in authors:
                na = etree.Element("author")
                na.text = a.value
                el[0].append(na)


    try:
        if upd_counter < 1:
            sys.exit(0)

        if args.verbose:
            print(etree.tostring(root, pretty_print=True).decode("utf-8"))

        with open(pddoc_path, 'wb') as f:
            f.write(etree.tostring(root, pretty_print=True, doctype='<?xml version="1.0" encoding="utf-8"?>'))

    except IOError:
        pass

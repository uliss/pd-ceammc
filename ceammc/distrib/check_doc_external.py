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

corrector = jamspell.TSpellCorrector()

def read_all_externals():
    return list(filter(lambda x: len(x), subprocess.check_output([EXT_LIST], stderr=subprocess.DEVNULL).decode().split('\n')))

def read_methods(name):
    try:
        return set(filter(lambda x: len(x) and x[0] != '@',
            subprocess.check_output([EXT_METHODS, name], stderr=subprocess.DEVNULL).decode().split('\n')))
    except(subprocess.CalledProcessError):
        cprint(f"[{name}] can't get methods", "red")
        return set()

def read_props(name):
    try:
        s = subprocess.check_output([EXT_PROPS, name], stderr=subprocess.DEVNULL).decode()
        js = json.loads(s)
        return set(js.keys())
    except(subprocess.CalledProcessError):
        cprint(f"[{name}] can't get properties", "red")
        return set()

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

    doc_methods = set()
    doc_props = set()

    # parse methods
    for appt in root:
        for x in appt:
            if x.tag == "methods":
                for m in x:
                    if m.tag != "method" or m.get("example", False) or m.get("internal", False):
                        continue
                    doc_methods.add(m.attrib["name"].split(' ')[0])

            if x.tag == "properties":
                for p in x:
                    if p.tag != "property":
                        continue

                    doc_props.add(p.attrib["name"])

    if args.methods:
        ext_methods = read_methods(ext_name)
        # print(doc_methods)
        # print(ext_methods)
        ignored_methods = {'dump', 'dsp', 'signal', 'mouseup', 'mouseenter', 'dialog', 'iscicm',
        'zoom', 'mousewheel', 'mousemove', 'mousedown', 'mouseleave',
        'symbol', 'float', 'bang', 'dblclick', 'list', 'dsp_add', 'loadbang', 'click'}
        undoc_methods = ext_methods - doc_methods - ignored_methods
        unknown_methods = doc_methods - ext_methods
        if len(undoc_methods):
            cprint(f"[{ext_name}] undocumented methods: {undoc_methods}", 'magenta')

        if len(unknown_methods):
            cprint(f"[{ext_name}] unknown methods in doc: {unknown_methods}", 'yellow')

    if args.props:
        ignored_props = {'@*'}

        ext_props = read_props(ext_name)
        undoc_props = ext_props - doc_props - ignored_props
        unknown_props = doc_props - ext_props

        if len(undoc_props):
            cprint(f"[{ext_name}] undocumented properties: {undoc_props}", 'magenta')

        if len(unknown_props):
            cprint(f"[{ext_name}] unknown properties in doc: {unknown_props}", 'yellow')

    if args.spell:
        corrector.LoadLangModel('ceammc.bin')
        cprint(f"checking [{ext_name}] ...", "blue")
        check_spell(root)

#!/usr/bin/env python3
import os
import re
import logging
import xml.etree.ElementTree as ET

logging.addLevelName(logging.WARNING, "\033[1;31m%s\033[1;0m " % logging.getLevelName(logging.WARNING))
logging.addLevelName(logging.ERROR, "\033[1;41m%s\033[1;0m " % logging.getLevelName(logging.ERROR))

ignored = set(['prepend', '_tcl.version'])

def allExternalsWithAliases():
    res = set()
    with open("../ceammc_objects.txt", "r") as f:
        for l in f.readlines():
            res.add(l.strip())

    return res

def allDocFilesInCmake():
    r_obj = re.compile(r'\s*([\w\d\.~^+-@]+)\s*')
    doc_obj = set()
    with open("docs.cmake", "r") as f:
        for l in f.readlines():
            m = r_obj.match(l)
            if not m:
                continue
            doc_obj.add(m.group(1))

    return doc_obj

def allDocFilesWithAliases():
    files = filter(lambda x: x.endswith(".pddoc"), os.listdir("."))
    res = set()
    for f in files:
        doc_name = os.path.splitext(f)[0]
        res.add(doc_name)

        tree = ET.parse(f)
        root = tree.getroot()
        for alias in root.iter('alias'):
            res.add(alias.text)

    return res

def allDocFilesOnDisk():
    files = filter(lambda x: x.endswith(".pddoc"), os.listdir("."))
    res = set()
    for f in files:
        doc_name = os.path.splitext(f)[0]
        res.add(doc_name)

    return res

def not_in_project():
    files = allDocFilesOnDisk() - allDocFilesInCmake() - ignored
    if len(files) > 0:
        logging.warning("this pddoc files are not in cmake project (no PD-help will be generated for them):")
        for f in files:
            print("    {0}".format(f))

def non_documented():
    files = allExternalsWithAliases() - allDocFilesWithAliases() - ignored

    if len(files) > 0:
        logging.warning("this objects are not documented:")
        for f in sorted(files):
            print("    {0}".format(f))

        logging.warning("undocumented objects: %s", len(files))

if __name__ == '__main__':
    not_in_project()
    non_documented()


#!/usr/bin/env python
import os
import re
import logging

logging.addLevelName(logging.WARNING, "\033[1;31m%s\033[1;0m" % logging.getLevelName(logging.WARNING))
logging.addLevelName(logging.ERROR, "\033[1;41m%s\033[1;0m" % logging.getLevelName(logging.ERROR))

def src_in_project():
    res = set()
    with open("../ceammc_objects.txt", "r") as f:
        for l in f.readlines():
            res.add(l.strip())

    return res

def pddoc_in_project():
    r_obj = re.compile(r'\s*([\w]+(\.|_)[\w\d_~]+)\s*')
    doc_obj = set()
    with open("CMakeLists.txt", "r") as f:
        for l in f.readlines():
            m = r_obj.match(l)
            if not m:
                continue
            doc_obj.add(m.group(1))

    return doc_obj

def pddoc_in_dir():
    files = filter(lambda x: x.endswith(".pddoc"), os.listdir("."))
    res = set()
    for f in files:
        res.add(os.path.splitext(f)[0])

    return res

def main():
    d1 = pddoc_in_project()
    d2 = pddoc_in_dir()
    d3 = d2 - d1
    d4 = src_in_project()

    if len(d3) > 0:
        logging.warning("this files not in cmake project:")
        for f in d3:
            print "    {0}".format(f)


    d5 = d4 - d1
    if len(d5) > 0:
        logging.warning("this objects are not documented:")
        for f in sorted(d5):
            print "    {0}".format(f)

        logging.warning("undocumented objects: %s", len(d5))

if __name__ == '__main__':
    main()

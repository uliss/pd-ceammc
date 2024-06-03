#!/usr/bin/env python3
import sys
import lxml.etree as lx
import textwrap

LIB_PATH = "@PROJECT_SOURCE_DIR@/ceammc/ext/doc/ceammc_lib.xml"
OUT_FILE = None

exclude_cats = ()

def parse_category(cat):
    cname = cat.get("name")

    if cname in exclude_cats:
        return

    print("Processing category", '"' + cname + '"', "...")

    OUT_FILE.write("    {%s {\n" % cname)

    obj = list(filter(lambda x: x is not None, map(lambda x: x.get("name"), cat)))
    obj.sort()
    line = "\n".join(textwrap.wrap(" ".join(obj)))
    line = textwrap.indent(line, "        ")
    OUT_FILE.write("%s\n" % line)

    OUT_FILE.write("    }}\n")

def main():
    tree = lx.parse(LIB_PATH)
    root = tree.getroot()

    if len(root) == 0:
        print("Empty library: ", LIB_PATH)
        return

    OUT_FILE.write("{ceammc\n")

    for cat in root:
        if cat.tag == "category":
            parse_category(cat)

    OUT_FILE.write("}\n")

if __name__ == '__main__':
    OUT_FILE = open("ceammc_popup_menu_ceammc.tcl", "w")
    main()
    OUT_FILE.close()

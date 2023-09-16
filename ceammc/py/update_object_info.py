# This Python file uses the following encoding: utf-8

import clang.cindex
import argparse
import typing
import os.path as path
import re
from lxml import etree as ET
from termcolor import cprint

def filter_node_list_by_node_kind(nodes: typing.Iterable[clang.cindex.Cursor], kinds: list) -> typing.Iterable[clang.cindex.Cursor]:
    result = []

    for i in nodes:
        if i.kind in kinds and i.is_definition():
            result.append(i)

    return result

def clean_str(str):
    return re.sub(r"\s+", " ", str.translate({ord(c):None for c in '\n\t\r'}).strip(), flags=re.UNICODE)

def find_external_name(lines):
    for l in lines:
        if l.find("Factory") == -1:
            continue

        z = re.match(r"\s+.+ obj\(\"(.+)\"(\,.+)?\);\n", l)
        if z:
            return z.group(1)

    return ""

def get_pddoc_description(xml):
    el = xml.find("./object/meta/description")
    if el is not None:
        return clean_str(el.text)
    else:
        return ""

def get_pddoc_category(xml):
    el = xml.find("./object/meta/category")
    if el is not None:
        return clean_str(el.text)
    else:
        return ""

def get_pddoc_keywords(xml):
    el = xml.find("./object/meta/keywords")
    if el is not None:
        return ", ".join(map(lambda x: '"'+escape_cxx_string(x)+'"', el.text.split(" ")))
    else:
        return ""

def update_description(cxx_file, cxx_desc, cxx_line, pddoc_xml):
    desc = get_pddoc_description(pddoc_xml)
    if cxx_desc != desc:
        print("Should update description: ", desc)

def get_external_pddoc(name):
    pddoc_fname = f"./doc/{name}.pddoc"
    if not path.exists(pddoc_fname):
        cprint(f"pddoc file not found: '{pddoc_fname}'", color='red')
        return None

    tree = ET.parse(pddoc_fname)
    return tree.getroot()

def escape_cxx_string(str):
    return str.replace('"','\\"')

def update_info(cxx_file, fn_name, line_start, line_end):
    with open(cxx_file, 'r') as f:
        all_lines = f.readlines()
        fn_lines = list(map(lambda x: x, all_lines[line_start:line_end-1]))
        ext_name = find_external_name(fn_lines)
        if len(ext_name) == 0:
            cprint(f"can't find external name in file: '{cxx_file}'", color='red')
            return

        cprint(f"object name: [{ext_name}]", color='blue')
        xml = get_external_pddoc(ext_name)
        if xml is None:
            return

        has_desc = False
        for k, v in enumerate(fn_lines):
            z = re.match(r"\s+obj\.setDescription\(\"(.+)\"\);\n", v)
            if z:
                update_description(cxx_file, z.group(1), line_start + k, xml)
                has_desc = True

        has_category = False
        for k, v in enumerate(fn_lines):
            z = re.match(r"\s+obj\.setCategory\(\"(.+)\"\);\n", v)
            if z:
#                update_description(cxx_file, z.group(1), line_start + k, xml)
                has_category = True

        has_keywords = False
        for k, v in enumerate(fn_lines):
            z = re.match(r"\s+obj\.setKeywords\({(.+)}\);\n", v)
            if z:
#                update_description(cxx_file, z.group(1), line_start + k, xml)
                has_keywords = True

        write_file = False

        if not has_keywords:
            cprint("no has_keywords in C++", color='yellow')
            keys = get_pddoc_keywords(xml)
            all_lines.insert(line_end-1, f"    obj.setKeywords({{{keys}}});\n")
            write_file = True

        if not has_category:
            cprint("no category in C++", color='yellow')
            cat = escape_cxx_string(get_pddoc_category(xml))
            all_lines.insert(line_end-1, f"    obj.setCategory(\"{cat}\");\n")
            write_file = True

        if not has_desc:
            cprint("no description in C++", color='yellow')
            desc = escape_cxx_string(get_pddoc_description(xml))
            all_lines.insert(line_end-1, f"    obj.setDescription(\"{desc}\");\n")
            all_lines.insert(line_end-1, "\n")
            write_file = True

        if write_file:
            with open(cxx_file, 'w') as ff:
                f.close()
                ff.write("".join(all_lines))
                ff.close()


if __name__ == "__main__":
    clang.cindex.Config.set_library_path("/opt/local/libexec/llvm-14/lib")
    parser = argparse.ArgumentParser()
    parser.add_argument("CXX_SRC", help="C++ source file")
#    parser.add_argument("-V", "--verbose", type=set_true, help="print verbose output")

    args = parser.parse_args()
    cprint(f"c++ file: '{args.CXX_SRC}'", color='blue')

    index = clang.cindex.Index.create()
    clang_args = ['-std=c++11', '-I' + path.dirname(args.CXX_SRC), '-I./src/lib']
    cprint(f"clang args: {clang_args}", color='blue')
    translation_unit = index.parse(args.CXX_SRC, args=clang_args)

    for node in translation_unit.cursor.get_children():
        if node.kind != clang.cindex.CursorKind.FUNCTION_DECL:
            continue

        if not node.is_definition():
            continue

        if not node.spelling.startswith("setup_"):
            continue

        for stm in node.get_children():
            update_info(args.CXX_SRC, node.spelling, stm.extent.start.line, stm.extent.end.line)



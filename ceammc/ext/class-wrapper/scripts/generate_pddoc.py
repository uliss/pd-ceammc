#!/usr/bin/env python3

import sys
import os
import re
from argparse import ArgumentParser
from termcolor import colored, cprint
import json

import write_pddoc as docwrite
from common_functions import error_msg, warning_msg

def generate_pddoc_db(info, db_fp):
    if "constructor" in info:     
        db_fp.write("{0}.new".format(info["pd_class"].lower()))
        db_fp.write("\t\t" + ".")
        db_fp.write("\t\t.")
        db_fp.write("\n")

    for m in info["methods"]:
        m_info = info["methods"][m]
        db_fp.write(m_info["doc_name"])

        if "pd_aliases" in m_info:
            for a in m_info["pd_aliases"]:
                db_fp.write(f",{a}")

        db_fp.write("\t\t" + "." * int(len(m_info["doc_inlets"])))
        db_fp.write("\t\t" + "." * int(len(m_info["doc_outlets"])))
        db_fp.write("\n")

def generate_pddoc(info):
    update_list = list()

    if "constructor" in info:
        update_list += docwrite.classObject(**info)

    for m in info["methods"]:
        m_info = info["methods"][m]
        update_list += docwrite.classMethodObject(info["library"],
                                                  authors=info["authors"],
                                                  version=info["version"],
                                                  license=info["license"], **m_info)

    return update_list

def generate_completion(info, fp):
   if "constructor" in info:
       fp.write("{0}.new\n".format(info["pd_class"].lower()))

   for m in info["methods"]:
       m_info = info["methods"][m]
       fp.write(m_info["doc_name"] + "\n")

       if "pd_aliases" in m_info:
           for a in m_info["pd_aliases"]:
               fp.write(f"{a}\n")

def make_pddoc_list(info):
    res = list()

    for entry in info:
        if "constructor" in entry:
            res.append("{0}.new".format(entry["pd_class"].lower()))

        for m in entry["methods"]:
            m_info = entry["methods"][m]
            res.append(m_info["doc_name"])

    return res

def main():
    parser = ArgumentParser()
    parser.add_argument("-m", "--module-name", dest="mod_name", help="module name", required=True)
    parser.add_argument("-d", "--output-dir", dest="out_dir", help="output directory", required=True)

    args = parser.parse_args()
    mod_name = args.mod_name
    out_dir = args.out_dir

    # JSON library info path
    json_path = f"{out_dir}/library.json"
    if not os.path.exists(json_path):
        error_msg(f"interface file not found: \"{json_path}\"")
        sys.exit(1)

    with open(json_path, 'r') as json_fp:
        info = json.load(json_fp)

    lib_name = mod_name
    lib_version = info["version"]
    lib_license = info["license"]
    lib_authors = info["authors"]

    docwrite.__outputDir__ = f"{out_dir}/doc"

    update_list = list()

    with open(f"{out_dir}/doc/{mod_name}.db", "w") as db_fp:
        for entry in info["classes"]:
            entry["version"] = lib_version
            entry["authors"] = lib_authors
            entry["name"] = lib_name
            entry["license"] = lib_license
            entry["library"] = lib_name
            # generate db file
            generate_pddoc_db(entry, db_fp)
            # generate pddoc
            update_list += generate_pddoc(entry)

    # completion
    with open(f"{out_dir}/doc/completion_{mod_name}.txt", "w") as cmpl_fp:
        for entry in info["classes"]:
            generate_completion(entry, cmpl_fp)

    # remove obsolete files
    pddoc_list = make_pddoc_list(info["classes"])
    for f in os.listdir(docwrite.__outputDir__):
        if f.endswith(".pddoc"):
            name = os.path.splitext(f)[0]
            if name not in pddoc_list:
                os.remove(os.path.join(docwrite.__outputDir__, f))

    # output updated pddoc files
    print("\n".join(update_list))

if __name__ == "__main__":
    main()

#!/usr/bin/env python3

import filecmp
import os.path
import sys
import argparse
from termcolor import colored

APP = "/Applications/Pd-ceammc_2023.10.app"

# from CMake
SRC_DIR = "@PROJECT_SOURCE_DIR@"
BUILD_DIR = "@PROJECT_BINARY_DIR@"
BUNDLE = "@PROJECT_BINARY_DIR@/dist/@BUNDLE@"
TK_VERSION = "@TK_VERSION@"
CEAMMC_LIB_VERSION = "@CEAMMC_LIB_VERSION@"

CEAMMC_DIR = "/Contents/Resources"


def check_dir(dir: str):
    if not os.path.exists(dir) or not os.path.isdir(dir):
        print("{} directory not found: '{}'".format(colored("[Error]", "red"), dir))
        sys.exit(1)


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="diff between Pd-ceammc bundles")
    parser.add_argument("--dylib", action="store_true", help="show only dylib diff")
    parser.add_argument(
        "--app", type=str, default=APP, help="specify Pd-ceammc app directory"
    )

    args = parser.parse_args()

    dir1 = args.app + CEAMMC_DIR
    dir2 = BUNDLE + CEAMMC_DIR

    print("compare '{}' and '{}'".format(colored(dir1, "blue"), colored(dir2, "blue")))
    check_dir(dir1)
    check_dir(dir2)

    dir_cmp = filecmp.dircmp(dir1, dir2)
    dylib_old = set()
    dylib_new = set()
    
    files_removed = set()
    files_added = set()
    
    def cmp(cmp_dir1, cmp_dir2, dir_cmp):
        for name in dir_cmp.left_only:
            if name.endswith(".dylib"):
                dylib_old.add(name.split(".")[0])
            elif args.dylib:
                continue
            
            sym_dir=''
            if os.path.isdir(os.path.join(cmp_dir1, name)):
                sym_dir = '/'
        
            prefix = cmp_dir1[len(dir1):]    
            files_removed.add(os.path.join(prefix, name) + sym_dir)

        for name in dir_cmp.right_only:
            if name.endswith(".dylib"):
                dylib_new.add(name.split(".")[0])
            elif args.dylib:
                continue
            
            sym_dir=''
            if os.path.isdir(os.path.join(cmp_dir2, name)):
                sym_dir = '/*'
                
            prefix = cmp_dir2[len(dir2):]
            files_added.add(os.path.join(prefix, name) + sym_dir)
            
        for name in dir_cmp.subdirs.values():
            subdir1 = os.path.join(cmp_dir1, name.left)
            subdir2 = os.path.join(cmp_dir2, name.right)
            cmp(subdir1, subdir2, name)

    cmp(dir1, dir2, dir_cmp)

    dylib_removed = dylib_old - dylib_new
    dylib_added = dylib_new - dylib_old
    
    for f in sorted(list(files_removed)):
        print("\t{} {}".format(colored("-", "red"), f))
        
    for f in sorted(list(files_added)):
        print("\t{} {}".format(colored("+", "green"), f))

    for x in sorted(list(dylib_removed)):
        print("\tdll removed: {}".format(colored(x, "red")))

    for x in sorted(list(dylib_added)):
        print("\tdll added:   {}".format(colored(x, "green")))
        

#!/usr/bin/env python3

import sys
import os
import re
import subprocess
import shutil

OBJDUMP_BIN = "@OBJDUMP_EXE@"
MINGW_PREFIX = b"@CEAMMC_MINGW_PREFIX@"
DEST_DIR = b"@CMAKE_INSTALL_PREFIX@/@PD_EXE_INSTALL_PATH@"
DLL_SET = set()


def find_all_bins():
    pattern = re.compile(r'.+\.(dll|m_i386|m_amd64|exe)$')

    bins = set()
    bins.add("@PROJECT_BINARY_DIR@/src/pdsend.exe")
    bins.add("@PROJECT_BINARY_DIR@/src/pdreceive.exe")
    bins.add("@PROJECT_BINARY_DIR@/src/pd.exe")
    bins.add("@PROJECT_BINARY_DIR@/src/pd.dll")

    for root, dirnames, filenames in os.walk('@CMAKE_INSTALL_PREFIX@'):
        for f in filenames:
            if pattern.match(f):
                bins.add(os.path.join(root, f))

    return bins


def find_bin_dlls(path):
    #    print(os.path.basename(path))
    for line in subprocess.check_output([OBJDUMP_BIN, "-p", path]).split(b'\r\n'):
        idx = line.find(b'DLL Name: ')
        if idx > 0:
            dll = line[idx+10:]
            dll_path = os.path.join(MINGW_PREFIX, b"bin", dll)
            if dll_path in DLL_SET:
                continue

            if os.path.exists(dll_path):
                DLL_SET.add(dll_path)
                print(" + ", dll.decode())
                find_bin_dlls(dll_path)


if __name__ == "__main__":
    bin_list = sorted(list(find_all_bins()))
    for f in bin_list:
        find_bin_dlls(f)

    for dll in sorted(DLL_SET):
        src = os.path.basename(dll).decode()
        dest = os.path.join(DEST_DIR, os.path.basename(dll)).decode()
        shutil.copyfile(dll, dest)
        print(f"'{src}' -> '{dest}'")

    sys.exit(0)

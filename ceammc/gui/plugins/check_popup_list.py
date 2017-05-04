#!/usr/bin/env python
import sys

def ceammc_list():
    f = open("../../ext/ceammc_objects.txt", 'r')
    lst = f.read().split()
    f.close()
    return lst

def core_list():
    f = open("../../ext/core_objects.txt", 'r')
    lst = f.read().split()
    f.close()
    return lst

def cream_list():
    f = open("../../extra/CreamLibrary/cream_objects.txt", 'r')
    lst = f.read().split()
    f.close()
    return lst

def menu_list(fname):
    stack = []
    lines = []

    f = open(fname)
    ln = f.read()
    f.close()

    dir = 0

    for ch in ln:
        if ch == '{':
            dir = 1
            stack.append("")
            continue
        elif ch == '}':
            if len(stack) > 0 and dir == 1:
                lines.append(stack[-1])

            dir = -1
            stack.pop()
            continue

        if len(stack) > 0:
            stack[-1] += ch

    lst = ' '.join(lines).split()
    lst.sort()
    return lst

def main():
    if len(sys.argv) != 2:
        print "Usage:", sys.argv[0], "FILENAME"
        sys.exit()

    c = ceammc_list()
    cream = cream_list()
    core = core_list()
    m = menu_list(sys.argv[1])

    for obj in m:
        if obj in core:
            continue
        elif obj in c:
            continue
        elif obj in cream:
            continue
        elif obj in ('bang', 'float', 'symbol'):
            continue
        elif obj.startswith('exp.'):
            continue

        print obj, "is not found"


if __name__ == '__main__':
    main()

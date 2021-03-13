#!/usr/bin/env python3

import sys
import os
import re

if __name__ == "__main__":
    if len(sys.argv) != 2:
        print("usage: {0}" % (sys.argv[0]))
        sys.exit(1)

    lines = []
    with open(sys.argv[1], 'r') as f:
        lines = f.readlines()

    n = 0
    for l in lines:
        if len(l) > 0 and l[0] == '#':
            m = re.match(r'#\s*([^\s]+)\s+(.*)', l)
            g1 = m.group(1)
            if g1 == "voice":
                msg = "speech @voice"
            else:
                msg = m.group(1)

            print("{0} {1};".format(msg, m.group(2)))
        elif len(l) > 0 and l[0] == '.':
            l = l.replace(',', ' \\, ')
            l = l.replace(':', '')

            m = re.match(r'\.([^\s]+)\s+(.*)', l)
            g1 = m.group(1)
            if g1 == "say":
                msg = "speech list"
            elif g1 == "sleep":
                msg = "sleep"
            else:
                msg = m.group(1)

            if n == 0:
                print("{1} {2};".format(n, msg, m.group(2)))
            else:
                print("{0} {1} {2};".format(n, msg, m.group(2)))

            n += 1

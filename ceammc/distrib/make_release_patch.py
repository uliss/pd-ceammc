#!/usr/bin/env python3

import sys
import os
import re

STATE_CTL_BEGIN = 0
STATE_CTL_NONE = -1

ctl_state = STATE_CTL_NONE

def ctl_begin():
    global ctl_state
    ctl_state = STATE_CTL_BEGIN
    print("1 # [;")

def ctl_end():
    global ctl_state
    ctl_state = STATE_CTL_NONE
    print("next 1;")
    print("# ];")


if __name__ == "__main__":
    if len(sys.argv) != 2:
        print("usage: {0}" % (sys.argv[0]))
        sys.exit(1)

    lines = []
    with open(sys.argv[1], 'r') as f:
        lines = f.readlines()

    n = 0
    for l in lines:
        if len(l) < 1:
            continue

        if l[0:2] == "//": # comment
            continue

        elif l[0] == '#': # CTL

            if ctl_state == STATE_CTL_NONE:
                ctl_begin()

            m = re.match(r'#\s*([^\s]+)\s+(.*)', l)
            g1 = m.group(1)
            if g1 == "voice":
                msg = "speech @voice"
            else:
                msg = m.group(1)

            print("{0} {1};".format(msg, m.group(2)))

        elif l[0] == '.': # command

            if ctl_state == STATE_CTL_BEGIN:
                ctl_end()

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

            print("{0} {1} {2};".format(n, msg, m.group(2)))
            n += 10

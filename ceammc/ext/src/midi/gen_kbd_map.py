#!/usr/bin/env python3

import sys
import re
import os.path as p

all_keys = dict() 

def add_keys(n, keys):
    i = int(n)
    for k in keys:
        ch = ord(k)
        all_keys[ch] = i
        i += 1

if __name__ == "__main__":
    if len(sys.argv) != 2:
        print("usage {} FILE".format(sys.argv[0]))
        sys.exit(2) 

    fname = sys.argv[1]
    with open(fname) as f:
        for ln in f.readlines():
            res = re.match(r'(-?\d+) ([^ ]+)\n', ln)
            if res is not None:
                add_keys(res.group(1), res.group(2))

    var_name = p.splitext(p.basename(fname))[0] 
    print("// this is autogenerated file, do not edit!")
    print(f'#ifndef midi_{var_name}_h_')
    print(f'#define midi_{var_name}_h_')
    print('#include <unordered_map>')
    print('#include <cstdint>')
    print(f'static const std::unordered_map<uint32_t, int8_t> {var_name} = {{');
    for k in sorted(all_keys.keys()):
        v = all_keys[k]
        c = chr(k)
        print(f'    {{ {k}, {v} }}, // "{c}"')

    print('};')
    print(f'#endif // midi_{var_name}_h_')

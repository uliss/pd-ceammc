#!/usr/bin/env python

import os.path
import subprocess
from generate_math import UNARY_EXT
from mako.template import Template

def parse_section(name, dict):
    name = name.upper()
    if name not in dict:
        return ""

    return map(lambda x: x.strip(), dict[name])

def parse_man(txt):
    lines = txt.split("\n")
    sects = {}
    last_sect = None
    for l in lines:
        if len(l) > 0 and l[0].isupper():
            n = l.strip().split(' ')[0]
            last_sect = n
            sects[n] = []
        else:
            if not last_sect:
                continue

            sects[last_sect].append(l)

    res = {}
    name = parse_section('name', sects)
    res['name'] = ' '.join(name).split('--')[1].strip()

    see = parse_section('see', sects)
    if len(see) > 0:
        res['see'] = ' '.join(see[:1]).replace('(3)', '').split(', ')[:4]

    descr = parse_section('description', sects)
    res['descr'] = '\n'.join(descr).split('\n\n')

    spec = parse_section('special', sects)
    if len(spec) > 0:
        res['spec'] = '\n'.join(spec).split('\n\n')

    return res

def generate_doc(func):
    cmd = ['man', '-P', 'col -bx', "3", func]
    f = subprocess.check_output(cmd)
    data = parse_man(f)
    data['func'] = func
    data['cat'] = 'math'
    data['obj'] = 'math.' + func
    data['version'] = "0.1"

    fname = 'math.{0}.pddoc'.format(func)
    content = Template(TEMPLATE).render(**data)
    print "... {0} generated".format(fname)

    with open(fname, 'w') as f:
        f.write(content)

def main():
    for t in UNARY_EXT:
        n,_,_ = t
        generate_doc(n)

TEMPLATE = '''<?xml version="1.0" encoding="UTF-8"?>
<pddoc version="1.0">
    <object name="${obj|x}">
        <title>${obj|x}</title>
        <meta>
            <authors>
                <author>Serge Poltavsky</author>
            </authors>
            <description>${name|x}</description>
            <license>GPL3</license>
            <library>ceammc</library>
            <category>${cat|x}</category>
            <keywords>${cat|x} ${func|x}</keywords>
            % if len(see):
            <also>
                % for s in see:
                <see>${cat|x}.${s|x}</see>
                % endfor
            </also>
            % endif
            <since>0.1</since>
        </meta>
        <info>
            % for p in descr:
            <par>${p|x}</par>
            % endfor
            % if spec:
            <par>Special values:</par>
                % for p in spec:
            <par>${p|x}</par>
                % endfor
            % endif
        </info>
        <inlets>
            <inlet>
                <xinfo on="float">input value</xinfo>
            </inlet>
        </inlets>
        <outlets>
            <outlet>result value</outlet>
        </outlets>
        <example>
            <pdascii>
<![CDATA[
[F]
|
[${obj}]
|
[F]
]]>
            </pdascii>
        </example>
    </object>
</pddoc>
'''

if __name__ == '__main__':
    main()


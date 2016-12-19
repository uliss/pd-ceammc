#!/usr/bin/env python
# -*- coding: utf-8 -*-#

import os.path
import imp
import argparse

CWD = os.path.dirname(__file__)
ceammc = imp.load_source('module.name', os.path.join(CWD, 'ceammc/template.py'))

parser = argparse.ArgumentParser(description='Generate PureData ceammc math extensions.')
parser.add_argument('module', metavar='MODULE', help='Module name')
parser.add_argument('name', metavar='NAME', help='extension name')

parser.add_argument('--headers', dest='headers', help='#include headers')
parser.add_argument('--methods', dest='methods', help='generated methods')
parser.add_argument('--cpp',  dest='cpp', action='store_true', help='generate C++ style extension')
parser.add_argument('--free', dest='free', action='store_true', help='generate free function')


group_math = parser.add_argument_group('math')
group_math.add_argument('--f32', dest='func32', help='32-bit float function')
group_math.add_argument('--f64', dest='func64', help='64-bit double function')
group_math.add_argument('--type', dest='type', choices=['none', 'empty', 'unary', 'const'],
                        help='generate function of given type')
group_math.add_argument('--code', dest='code', help='generated code')

args = parser.parse_args()
methods = []


def generate_common(args):
    headers = None
    if args.headers:
        headers = args.headers.split(',')

    g = ceammc.PdExtension(args.module, args.name, headers=headers)
    methods = ['float', 'list']
    if args.methods:
        methods = args.methods.split(',')

    g.gen_free = args.free
    g.gen_cpp = args.cpp
    g.generate(methods)


if args.module == 'math':
    code = None
    func32 = None
    func64 = None

    if args.type == 'unary':
        code = args.code
        if args.func32:
            func32 = args.func32
        if args.func64:
            func64 = args.func64

        if not func32:
            code = ' '

        g = ceammc.PdMathUnaryExtension(args.name, func32, func64, code)
        g.gen_cpp = args.cpp
        g.generate()
    elif args.type == 'const':
        g = ceammc.PdMathConstExtension(args.name, args.code)
        g.generate()
    elif args.type == 'empty':
        code = ' '
        g = ceammc.PdMathUnaryExtension(args.name, None, None, code)
        g.generate()
    else:
        generate_common(args)
else:
    generate_common(args)


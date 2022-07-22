#!/usr/bin/env python
# -*- coding: utf-8 -*-#

import os.path
import importlib.util
import argparse

CWD = os.path.dirname(__file__)
ceammc_spec = importlib.util.spec_from_file_location('module.name', os.path.join(CWD, 'ceammc/template.py'))
ceammc = importlib.util.module_from_spec(ceammc_spec)
ceammc_spec.loader.exec_module(ceammc)
ceammc_cpp_spec = importlib.util.spec_from_file_location('module.name2', os.path.join(CWD, 'ceammc/cppexternal.py'))
ceammc_cpp = importlib.util.module_from_spec(ceammc_cpp_spec)
ceammc_cpp_spec.loader.exec_module(ceammc_cpp)

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

    g = ceammc.PdExternal(args.module, args.name, headers=headers)
    cpp = ceammc_cpp.MathUnaryCppExternalBoth(args.module, args.func64)
    methods = ['float', 'list']
    if args.methods:
        methods = args.methods.split(',')

    g.gen_free = args.free
    g.gen_cpp = args.cpp

    if args.cpp:
        cpp.generate(methods)
    else:
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

        g = ceammc_cpp.MathUnaryCppExternalBoth(args.name, func64)
        g.generate(['float', 'list'])
    elif args.type == 'const':
        g = ceammc.PdMathConstExternal(args.name, args.code, cpp=args.cpp)
        g.generate()
    elif args.type == 'empty':
        code = ' '
        g = ceammc.PdMathUnaryExternal(args.name, None, None, code)
        g.generate()
    else:
        generate_common(args)
else:
    generate_common(args)


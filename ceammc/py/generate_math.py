#!/usr/bin/env python

import os.path
import subprocess

UNARY_EXT = [
    ('abs', 'fabsf', 'fabs'),
    ('acos', 'acosf', 'acos'),
    ('acosh', 'acoshf', 'acosh'),
    ('asin', 'asinf', 'asin'),
    ('asinh', 'asinhf', 'asinh'),
    ('atan',  'atanf',  'atan'),
    ('atanh', 'atanhf', 'atanh'),
    ('cbrt',  'cbrtf',  'cbrt'),
    ('ceil',  'ceilf',  'ceil'),
    ('cos',   'cosf',   'cos'),
    ('cosh',  'coshf',  'cosh'),
    ('erf',   'erf',   'erf'),
    ('exp',   'expf',   'exp'),
    ('exp2',  'exp2f',  'exp2'),
    ('floor', 'floorf', 'floor'),
    ('gamma', 'tgamma', 'tgamma'),
    ('lgamma', 'lgamma', 'lgamma'),
    ('log',   'logf',   'log'),
    ('log10', 'log10f', 'log10'),
    ('log2',  'log2f',  'log2'),
    ('round', 'roundf', 'round'),
    ('sin',   'sinf',   'sin'),
    ('sinh',  'sinhf',  'sinh'),
    ('sqrt',  'sqrtf',  'sqrt'),
    ('tan',   'tanf',   'tan'),
    ('tanh',  'tanhf',  'tanh'),
    ('trunc', 'truncf', 'trunc')
]

CONST_EXT = [
    ('e',   'M_E')
]

SCRIPT = os.path.join(os.path.dirname(__file__), 'extension_generator.py')


def main():
    for ext in UNARY_EXT:
        name = ext[0]
        f = open("math_{}.cpp".format(name), "w")
        cmd = [SCRIPT, "--type", "unary", "--f32", ext[1], "--f64", ext[2], "math", name]
        subprocess.call(cmd, stdout=f)
        f.close()
        print(f"math_{name}.cpp generated")


    for ext in CONST_EXT:
        name = ext[0]
        value = ext[1]
        f = open("math_{}.cpp".format(name), "w")
        cmd = [SCRIPT, "--type", "const", "--cpp", "--code", value, "math", name]
        subprocess.call(cmd, stdout=f)
        f.close()
        print(f"math_{name}.cpp generated")

if __name__ == '__main__':
    main()

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
    ('exp',   'expf',   'exp'),
    ('exp2',   'exp2f',   'exp2')
]

SCRIPT = os.path.join(os.path.dirname(__file__), 'extension_generator.py')

for ext in UNARY_EXT:
    name = ext[0]
    f = open("math_{}.c".format(name), "w")
    cmd = [SCRIPT, "--unary", "--f32", ext[1], "--f64", ext[2], "math", name]
    subprocess.call(cmd, stdout=f)
    f.close()
    print "math_{}.c generated".format(name)



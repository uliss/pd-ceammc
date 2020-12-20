import template


def generate_external(module, name, headers=None):
    if headers is None:
        headers = []

    g = template.PdExternal(module, name, headers)
    g.generate('common')


def generate_math_libc_extension(name, func32, func64):
    g = template.PdMathUnaryExternal(name, func32, func64)
    g.generate()

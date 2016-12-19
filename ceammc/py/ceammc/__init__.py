import template


def generate_extension(module, name, headers=None):
    if headers is None:
        headers = []

    g = template.PdExtension(module, name, headers)
    g.generate('common')


def generate_math_libc_extension(name, func32, func64):
    g = template.PdMathUnaryExtension(name, func32, func64)
    g.generate()

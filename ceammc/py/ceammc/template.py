# -*- coding: utf-8 -*-


class PdExternal(object):
    def __init__(self, module, extension, headers=None, gen_free=False, cpp=False):
        if headers is None:
            headers = []

        self.module = module
        self.extension = extension
        self.name_ = module + '_' + extension
        self.class_ = self.name_ + '_class'
        self.type_ = 't_' + self.name_
        self.headers = headers
        self.gen_free = gen_free
        self.gen_cpp = cpp

    def generate(self, methods='common'):
        self.generate_header()
        self.generate_struct()

        if isinstance(methods, basestring):
            if methods == 'all':
                methods = ['bang', 'float', 'symbol', 'list', 'pointer', 'anything']
            elif methods == 'common':
                methods = ['bang', 'float', 'symbol', 'list']
            elif methods == '':
                methods = ['bang', 'float']

        methods.sort()

        for m in methods:
            try:
                func = getattr(self, 'generate_' + m)
                func()
            except AttributeError:
                print(m, " not found")

        self.generate_new()
        if self.gen_free:
            self.generate_free()
        self.generate_setup(methods)

    def generate_header(self):
        print('#include "ceammc.h"')

        print("#include \"m_pd.h\"")
        for f in self.headers:
            print("#include <{}>".format(f))

        macro = '''
#define OBJ_NAME "{mod}.{ext}"'''
        print(macro.format(mod=self.module, ext=self.extension))

    def generate_struct(self, fields=[]):
        res = '''
static t_class* {0};
typedef struct {1} {{
    t_object x_obj;{fields}
}} {2};'''

        if self.gen_cpp:
            res = '''
static t_class* {0};
struct {2} {{
    t_object x_obj;{fields}
}};'''

        print(res.format(self.class_, self.name_, self.type_, fields="\n".join(fields)))

    def generate_bang(self, code=''):
        res = '''
static void {name}_bang({type}* x)
{{
    {code}
}}'''
        print(res.format(name=self.name_, type=self.type_, code=code))

    def generate_float(self, code=''):
        res = '''
static void {name}_float({type}* x, t_floatarg f)
{{
    {code}
}}'''

        print(res.format(name=self.name_, type=self.type_, code=code))

    def generate_list(self, code=''):
        res = '''
static void {name}_list({type}* x, t_symbol* s, int argc, t_atom* argv)
{{
    {code}
}}'''
        print(res.format(name=self.name_, type=self.type_, code=code))

    def generate_symbol(self, code=''):
        res = '''
static void {name}_symbol({type}* x, t_symbol *s)
{{
    {code}
}}'''
        print(res.format(name=self.name_, type=self.type_, code=code))

    def generate_pointer(self, code=''):
        res = '''
static void {name}_pointer({type}* x, t_gpointer *pt)
{{
     {code}
}}'''

        print(res.format(name=self.name_, type=self.type_, code=code))

    def generate_anything(self, code=''):
        res = '''
static void {name}_anything({type} *x, t_symbol *s, int argc, t_atom *argv)
{{
    {code}
}}'''

        print(res.format(name=self.name_, type=self.type_, code=code))

    def generate_new(self, outlet_type='s_float', code=''):
        res = '''
static void* {name}_new()
{{
    {type}* x = ({type}*)pd_new({class_});
    outlet_new(&x->x_obj, &{outlet});
    {code}
    return (void*)x;
}}'''
        if(self.gen_cpp):
            res = '''
static void* {name}_new()
{{
    {type}* x = reinterpret_cast<{type}*>(pd_new({class_}));
    outlet_new(&x->x_obj, &{outlet});
    {code}
    return static_cast<void*>(x);
}}'''

        print(res.format(name=self.name_, type=self.type_, class_=self.class_, outlet=outlet_type, code=code))

    def generate_free(self, code=''):
        res = '''
static void {name}_free({type} *x)
{{
    {code}
}}'''
        print(res.format(name=self.name_, code=code, type=self.type_))

    def generate_setup(self, methods=None):
        # handle cpp
        if methods is None:
            methods = ['float', 'list']
        if self.gen_cpp:
            res = '\nextern '
        else:
            res = '\n'

        res += 'void setup_{0}_{1}()\n{{'.format(self.module, self.extension)

        free = '0';
        if self.gen_free:
            free = '{}_free'.format(self.name_)

        if self.gen_cpp:
            res += '''
    {class_} = class_new(gensym(OBJ_NAME),
        reinterpret_cast<t_newmethod>({name_}_new),
        reinterpret_cast<t_method>({free_}),
        sizeof({type_}), 0, A_NULL);'''.format(class_=self.class_,
                                                   type_=self.type_,
                                                   name_=self.name_,
                                                   free_=free,
                                                   mod=self.module,
                                                   ext=self.extension)
        else:
            res += '''
    {class_} = class_new(gensym(OBJ_NAME),
        (t_newmethod){name_}_new, (t_method){free_},
        sizeof({type_}), 0, A_NULL);'''.format(class_=self.class_,
                                                   type_=self.type_,
                                                   name_=self.name_,
                                                   free_=free,
                                                   mod=self.module,
                                                   ext=self.extension)

        if self.gen_cpp:
            for m in methods:
                res += "\n    class_add{0}({class_}, reinterpret_cast<t_method>({name_}_{0}));".format(m,
                                                                           class_=self.class_,
                                                                           name_=self.name_)
        else:
            for m in methods:
                res += "\n    class_add{0}({class_}, {name_}_{0});".format(m,
                                                                           class_=self.class_,
                                                                           name_=self.name_)

        res += "\n}\n"

        print(res)


class PdMathUnaryExternal(PdExternal):
    def __init__(self, name, func32=None, func64=None, func_code=None, headers=None):
        if headers is None:
            headers = ["math.h"]

        super(self.__class__, self).__init__("math", name, headers, False, False)
        self.func_code = func_code
        self.func32 = func32
        self.func64 = func64

    def generate(self):
        self.generate_header()
        self.generate_struct()
        self.generate_work_function()
        self.generate_float(code='outlet_float(x->x_obj.te_outlet, private_{0}(f));'.format(self.name_))
        self.generate_list(code='ceammc_atoms_map_float_to_outlet('
                                'x->x_obj.te_outlet, s, argc, argv, private_{0});'.format(self.name_))
        self.generate_new(outlet_type='s_float')
        self.generate_setup(['float', 'list'])

    def generate_work_function(self):
        print('\nstatic t_float private_{name}(t_float v) \n{{'.format(name=self.name_))

        if self.func_code:
            print('    {}'.format(self.func_code))
        else:
            if (self.func32 and self.func64) and (self.func32 != self.func64):
                print('''#if PD_FLOATSIZE == 32
    return {func32}(v);
#elif PD_FLOATSIZE == 64
    return {func64}(v);
#else
#error "Unsupported PD_FLOATSIZE"
#endif'''.format(func32=self.func32, func64=self.func64))

            if (self.func32 and not self.func64) or (self.func32 == self.func64):
                print('    return {}(v);'.format(self.func32))

        print('}')


class PdMathConstExternal(PdExternal):
    def __init__(self, name, const_value, headers=None, cpp=True):
        if headers is None:
            headers = ["math.h"]

        super(self.__class__, self).__init__("math", name, headers, False, cpp)
        self.const_value = const_value

    def generate(self):
        self.generate_header()
        self.generate_struct()
        self.generate_bang('outlet_float(x->x_obj.te_outlet, {});'.format(self.const_value))
        self.generate_new(outlet_type='s_float')
        self.generate_setup(['bang'])

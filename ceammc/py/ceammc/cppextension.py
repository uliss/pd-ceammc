# -*- coding: utf-8 -*-

class CppExtension(object):
    def __init__(self, module, extension):
        self.module = module
        self.extension = extension
        self.class_ = self.module.title() + self.extension.title();

    def generate(self, methods='common'):
        self.generate_header()
        self.generate_class_begin()

        if isinstance(methods, basestring):
            if methods == 'all':
                methods = ['bang', 'float', 'symbol', 'list', 'anything']
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
                print m, " not found"

        self.generate_class_end()
        self.generate_setup()

    def generate_header(self):
        print "#include \"ceammc_factory.h\""
        print "#include \"ceammc_object.h\""
        print ""
        print "using namespace ceammc;"
        print ""

    def generate_class_begin(self):
        res = '''
class {0} : public BaseObject
{{
public:
    {0}(const PdArgs& a) : BaseObject(a)
    {{
        // createInlet();
        // createOutlet();
        // createProperty();
        // parseArguments();
    }}'''

        print res.format(self.class_)

    def generate_class_end(self):
        print '''
};
'''

    def generate_bang(self, code=''):
        res = '''
    void onBang()
    {{
        {code}
    }}'''

        print res.format(code=code)

    def generate_float(self, code=''):
        res = '''
    void onFloat(float f)
    {{
        {code}
    }}'''

        print res.format(code=code)

    def generate_list(self, code=''):
        res = '''
    void onList(const AtomList& l)
    {{
        {code}
    }}'''
        print res.format(code=code)

    def generate_symbol(self, code=''):
        res = '''
    void onSymbol(t_symbol* s)
    {{
        {code}
    }}'''
        print res.format(code=code)

    def generate_anything(self, code=''):
        res = '''
    void onAny(t_symbol* sel, const AtomList& l)
    {{
        {code}
    }}'''

        print res.format(code=code)


    def generate_setup(self, methods=None):
        # handle cpp
        if methods is None:
            methods = ['float', 'list']

        res = '\nextern "C" void setup_{0}0x2e{1}()\n{{'.format(self.module, self.extension)

        res += '''
    ObjectFactory<{class_}> obj("{mod}.{ext}");'''.format(class_=self.class_, mod=self.module, ext=self.extension)

        res += "\n}\n"
        print res

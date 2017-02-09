#include "oop_common.h"

t_eclass* exp_class_class;

struct t_exp_class {
    t_ebox e_box;

    t_canvas* parent_canvas;
    //t_canvas *sub_canvas;

    OPClasses* op_class;

    t_atom* patch_a;
    int patch_c;

    t_outlet* out1;
};

#define OBJ_NAME "exp.class"

using namespace ceammc;

static void exp_class_read(t_exp_class* x, t_symbol*, int argc, t_atom* argv)
{
    if (argc < 1) {
        error("specify file name");
        return;
    }

    Atom a = Atom(argv[0]);
    x->op_class->ref()->readFile(a.asString(), x->parent_canvas);
}

static void exp_class_write(t_exp_class* x, t_symbol*, int argc, t_atom* argv)
{
    if (argc < 1) {
        //error("specify file name");

        if (x->op_class->ref()->canvas) {
            std::string class_name = x->op_class->ref()->class_name + ".class.pd";
            Atom a_class_name = Atom(gensym(class_name.c_str()));
            exp_class_write(x, 0, 1, AtomList(a_class_name).toPdData());
        }

        return;
    }

    Atom a = Atom(argv[0]);
    x->op_class->ref()->writeFile(a.asString(), x->parent_canvas);
}

#pragma mark -
#pragma mark dynamic

static void exp_class_newclass(t_exp_class* x, t_symbol*, int argc, t_atom* argv)
{
    if (argc < 1) {
        error("specify class name");
        return;
    }

    Atom a = Atom(argv[0]);

    x->op_class = new OPClasses(a.asString(), OBJ_NAME);
    if (!x->op_class->ref()) {
        x->op_class->ref() = new OPClass();
        x->op_class->ref()->class_name = a.asString();

        post("new class: %s", a.asString().c_str());
    }
}

static void exp_class_addproperty(t_exp_class* x, t_symbol*, int argc, t_atom* argv)
{
    if (argc < 2) {
        error("specify property name and alias");
        return;
    }

    AtomList list = AtomList(argc, argv);

    if (x->op_class->ref()) {
        x->op_class->ref()->addProperty(list[0].asString(), list[1].asString());

        post("new property: %s", list[0].asString().c_str());
    }
}

static void exp_class_addmethod(t_exp_class* x, t_symbol*, int argc, t_atom* argv)
{
    if (argc < 2) {
        error("specify method name and alias");
        return;
    }

    AtomList list = AtomList(argc, argv);

    if (x->op_class->ref()) {
        x->op_class->ref()->addMethod(list[0].asString(), list[1].asString());

        post("new method: %s", list[0].asString().c_str());
    }
}

#pragma mark -

static void exp_class_click(t_exp_class* x, t_symbol*, int argc, t_atom* argv)
{
    if (x->op_class->ref())
        if (x->op_class->ref()->canvas)
            canvas_vis(x->op_class->ref()->canvas, 1);
}

static void* exp_class_new(t_symbol* id, int argc, t_atom* argv)
{
    Atom a;

    if (argc < 1) {
        a = Atom(gensym(""));
    } else {
        a = argv[0];
    }

    if (argc > 0 && !a.isSymbol()) {
        error("bad class name!");
        return 0;
    }

    t_exp_class* x = reinterpret_cast<t_exp_class*>(eobj_new(exp_class_class));

    x->patch_a = (t_atom*)malloc(0);
    x->patch_c = 0;

    t_binbuf* d = binbuf_via_atoms(argc, argv);
    if (x && d) {
        ebox_attrprocess_viabinbuf(x, d);
    }

    x->parent_canvas = canvas_getcurrent();

    //should be read only for others
    x->op_class = new OPClasses(a.asString(), OBJ_NAME);

    if (!x->op_class->ref() && a.asString() != "") {
        x->op_class->ref() = new OPClass(a.asString());

        //attempt to load file
        std::string class_name = a.asString() + ".class.pd";
        Atom a_class_name = Atom(gensym(class_name.c_str()));
        //TODO move to oop_common
        exp_class_read(x, 0, 1, AtomList(a_class_name).toPdData());

        //create parent class
        if (argc > 1) {
            Atom a2 = argv[1];
            if (a2.isSymbol()) {
                OPClasses* par_classes = new OPClasses(a2.asString(), OBJ_NAME); //0; //OPClass::findBySymbol(a2.asSymbol());
                if (par_classes) {
                    if (par_classes->ref())
                        x->op_class->ref()->setParentClass(par_classes->ref());
                } else
                    error("parent class %s not found!", a2.asString().c_str());
            } else
                error("bad parent class name");
        }

    } else {
        post("empty class created");
    }

    //TODO multiple class save handling

    x->out1 = outlet_new((t_object*)x, &s_anything);
    ebox_ready((t_ebox*)x);

    return static_cast<void*>(x);
}

static void exp_class_free(t_exp_class* x)
{
    if (x->op_class->ref()) {
        //TODO
        if (x->op_class->ref()->canvas) {
            std::string class_name = x->op_class->ref()->class_name + ".class.pd";
            Atom a_class_name = Atom(gensym(class_name.c_str()));
            exp_class_write(x, 0, 1, AtomList(a_class_name).toPdData());

            canvas_free(x->op_class->ref()->canvas);

            x->op_class->ref() = 0;
        }

        delete x->op_class;
    }
}

extern "C" void setup_exp0x2eclass()
{

    exp_class_class = eclass_new((OBJ_NAME),
        reinterpret_cast<t_typ_method>(exp_class_new),
        reinterpret_cast<t_typ_method>(exp_class_free),
        sizeof(t_exp_class), 0, A_GIMME, 0);

    eclass_addmethod(exp_class_class, (t_typ_method)(exp_class_read), ("readclass"), A_GIMME, 0);
    eclass_addmethod(exp_class_class, (t_typ_method)(exp_class_write), ("writeclass"), A_GIMME, 0);

    eclass_addmethod(exp_class_class, (t_typ_method)exp_class_click, ("click"), A_NULL, 0);

    eclass_addmethod(exp_class_class, (t_typ_method)(exp_class_newclass), ("newclass"), A_GIMME, 0);

    eclass_addmethod(exp_class_class, (t_typ_method)(exp_class_addproperty), ("addproperty"), A_GIMME, 0);
    eclass_addmethod(exp_class_class, (t_typ_method)(exp_class_addmethod), ("addmethod"), A_GIMME, 0);

    eclass_register(CLASS_BOX, exp_class_class);
}

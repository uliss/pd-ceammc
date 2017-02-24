#include "oop_common.h"

#define OBJ_NAME "exp.method"
#define OBJ_OUT_NAME "exp.method_out"

t_eclass* exp_method_class;
t_eclass* exp_method_out_class;

using namespace ceammc;

struct t_exp_method {
    t_ebox e_box;

    t_canvas* parent_canvas;

    t_symbol* method_name;

    OPInstance* instance;

    t_outlet* out1;
    t_outlet* out2; //temporary - separate pointer output for objects not inside the instance
};

static void* exp_method_new(t_symbol* id, int argc, t_atom* argv)
{

    if (argc < 1) {
        error("missing method name argument");
        return 0;
    }

    t_exp_method* x = reinterpret_cast<t_exp_method*>(eobj_new(exp_method_class));
    x->parent_canvas = canvas_getcurrent();

    t_binbuf* d = binbuf_via_atoms(argc, argv);

    if (x && d) {
        ebox_attrprocess_viabinbuf(x, d);
    }

    Atom a = (argv[0]);
    x->method_name = a.asSymbol();

    x->instance = OPInstance::findByCanvas(x->parent_canvas);

    ebox_ready((t_ebox*)x);

    x->e_box.b_boxparameters.d_boxfillcolor = rgba_greylight;
    x->e_box.b_boxparameters.d_bordercolor = rgba_green;

    x->out1 = outlet_new((t_object*)x, &s_anything);

    if (x->instance)
        x->instance->addMethod(x->method_name, x->out1);

    //add method for dynamic class
    //TODO move later to separate singleton / static etc
    //now uses "__dynamicStub" to store unconnected methods

    if (!canvas_is_instance(x->parent_canvas)) {
        post("adding unconnected method: %s", a.asString().c_str());

        OPClasses* dyn_class = new OPClasses("__dynamicStub", OBJ_NAME);
        if (!dyn_class->ref()) {
            dyn_class->ref() = new OPClass();
            dyn_class->ref()->class_name = "__dynamicStub";
        }

        dyn_class->ref()->addMethodOutlet(x->method_name->s_name, x->out1);

        x->out2 = outlet_new((t_object*)x, &s_anything);
        dyn_class->ref()->addMethodPointerOutlet(x->method_name->s_name, x->out2);
    }
    
    OPClass *this_class = OPClass::findByCanvas(x->parent_canvas);
    if (this_class)
    {
        // TEMP
        this_class->addMethod(x->method_name->s_name, "");
    }

    return static_cast<void*>(x);
}

static void exp_method_free(t_exp_method* x, t_symbol* id, int argc, t_atom* argv)
{
    if (x->instance)
        x->instance->freeMethod(x->method_name);
}

#pragma mark -

static void* exp_method_out_new(t_symbol* id, int argc, t_atom* argv)
{

    if (argc < 1) {
        error("missing method name argument");
        return 0;
    }

    t_exp_method* x = reinterpret_cast<t_exp_method*>(eobj_new(exp_method_out_class));
    x->parent_canvas = canvas_getcurrent();

    t_binbuf* d = binbuf_via_atoms(argc, argv);

    if (x && d) {
        ebox_attrprocess_viabinbuf(x, d);
    }

    Atom a = (argv[0]);
    x->method_name = a.asSymbol();

    x->instance = OPInstance::findByCanvas(x->parent_canvas);

    ebox_ready((t_ebox*)x);

    x->e_box.b_boxparameters.d_boxfillcolor = rgba_greylight;
    x->e_box.b_boxparameters.d_bordercolor = rgba_green;

    return static_cast<void*>(x);
}

static void exp_method_out_free(t_exp_method* x, t_symbol* id, int argc, t_atom* argv)
{
    if (x->instance)
        x->instance->freeMethod(x->method_name);
}

static void exp_methodout_any(t_exp_method* x, t_symbol* id, int argc, t_atom* argv)
{
    if (x->instance) {
        AtomList list((Atom(x->method_name)));
        AtomList list2(argc, argv);
        list.append(list2);
        if (x->instance)
            x->instance->multipleOutput(list);
    }
}

#pragma mark -

extern "C" void setup_exp0x2emethod()
{

    exp_method_class = eclass_new((OBJ_NAME),
        reinterpret_cast<t_typ_method>(exp_method_new),
        reinterpret_cast<t_typ_method>(exp_method_free),
        sizeof(t_exp_method), CLASS_PATCHABLE, A_GIMME, 0);

    eclass_addmethod(exp_method_class, (method)(exp_methodout_any), ("anything"), A_GIMME, 0);

    eclass_register(CLASS_OBJ, exp_method_class);

    exp_method_out_class = eclass_new((OBJ_OUT_NAME),
        reinterpret_cast<t_typ_method>(exp_method_out_new),
        reinterpret_cast<t_typ_method>(exp_method_out_free),
        sizeof(t_exp_method), CLASS_PATCHABLE, A_GIMME, 0);

    eclass_addmethod(exp_method_out_class, (method)(exp_methodout_any), ("anything"), A_GIMME, 0);

    eclass_register(CLASS_OBJ, exp_method_out_class);
}

//
//  exp_aproperty.cpp
//  pd_ext
//
//  Created by Alex Nadzharov on 20/01/17.
//
//

#include "oop_common.h"

#define OBJ_NAME "exp.aproperty"

t_eclass* exp_aproperty_class;

typedef ceammc::GlobalData<ceammc::AtomList> LocalList;

using namespace ceammc;

struct t_exp_aproperty {
    t_ebox e_box;

    t_canvas* parent_canvas;

    t_symbol* property_name;

    OPInstance* instance;

    t_outlet *out1, *out2, *out3;
};

static void* exp_aproperty_new(t_symbol* id, int argc, t_atom* argv)
{

    if (argc < 1) {
        error("missing method name argument");
        return 0;
    }

    t_exp_aproperty* x = reinterpret_cast<t_exp_aproperty*>(eobj_new(exp_aproperty_class));
    x->parent_canvas = canvas_getcurrent();

    t_binbuf* d = binbuf_via_atoms(argc, argv);

    if (x && d) {
        ebox_attrprocess_viabinbuf(x, d);
    }

    Atom a = (argv[0]);
    x->property_name = a.asSymbol();

    x->instance = OPInstance::findByCanvas(x->parent_canvas);

    x->e_box.b_boxparameters.d_boxfillcolor = rgba_greylight;
    x->e_box.b_boxparameters.d_bordercolor = rgba_green;

    x->out1 = outlet_new((t_object*)x, &s_anything);
    x->out2 = outlet_new((t_object*)x, &s_anything); //setter bang
    x->out3 = outlet_new((t_object*)x, &s_anything); //getter bang

    if (x->instance) {
        x->instance->addProperty(x->property_name, x->out3, x->out2);
    }
    
    OPClass *this_class = OPClass::findByCanvas(x->parent_canvas);
    if (this_class)
    {
        // TEMP
        this_class->addProperty(x->property_name->s_name, "");
    }

    ebox_ready((t_ebox*)x);

    return static_cast<void*>(x);
}

static void exp_aproperty_free(t_exp_aproperty* x, t_symbol* id, int argc, t_atom* argv)
{
    if (x->instance) {
        x->instance->freeProperty(x->property_name);
    }
}

#pragma mark -

static void exp_aproperty_output(t_exp_aproperty* x)
{
    if (x->instance)
        ceammc::to_outlet(x->out1, (x->instance->getAtomListProperty(x->property_name)));
}

static void exp_aproperty_bang(t_exp_aproperty* x)
{
    exp_aproperty_output(x);
}

//rename
static void exp_aproperty_list(t_exp_aproperty* x, t_symbol* s, int argc, t_atom* argv)
{
    x->instance->setAtomListProperty(x->property_name, AtomList(argc, argv));
    exp_aproperty_output(x);
}

extern "C" void setup_exp0x2eaproperty()
{

    exp_aproperty_class = eclass_new((OBJ_NAME),
        reinterpret_cast<t_typ_method>(exp_aproperty_new),
        reinterpret_cast<t_typ_method>(exp_aproperty_free),
        sizeof(t_exp_aproperty), CLASS_PATCHABLE, A_GIMME, 0);

    eclass_addmethod(exp_aproperty_class, (method)(exp_aproperty_bang), ("bang"), A_GIMME, 0);
    eclass_addmethod(exp_aproperty_class, (method)(exp_aproperty_list), ("anything"), A_GIMME, 0);

    eclass_register(CLASS_BOX, exp_aproperty_class);
}
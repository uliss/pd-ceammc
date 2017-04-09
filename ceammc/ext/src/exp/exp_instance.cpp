#include "oop_common.h"

t_eclass* exp_instance_class;

typedef struct _exp_instance {
    t_ebox e_box;

    t_canvas* parent_canvas; //patch

    //t_canvas *local_canvas;         //instance

    OPClasses* op_class; //class
    OPInstance* instance;

    t_etext* txt;
    t_efont* fnt;

    t_inlet* in2;
    t_sample* buffer;
    int vec_size;

    t_outlet *out1, *out2;
} t_exp_instance;

#define OBJ_NAME "exp.instance"

using namespace ceammc;

static void exp_instance_delete(t_exp_instance* x)
{
    if (x->instance) {
        
        
        //temp
        //delete x->buffer;
        x->buffer=0;
        
        x->instance->freeSignal(gensym("out1"));
        x->instance->freeInstanceOut(x->out1);
        x->instance->release(); //no delete
        x->instance = 0;
        
    }
    
    
}

#pragma mark -

static void exp_instance_newinstance(t_exp_instance* x, t_symbol* id, int argc, t_atom* argv)
{
    if (argc < 1) {
        error("no class name provided!");
        return;
    }
    
    Atom a = argv[0];
    if (!a.isSymbol()) {
        error("bad class name!");
        return;
    }

    x->op_class = new OPClasses(a.asString(), OBJ_NAME);

    if (!x->op_class)
    {
        error("class not found!");
        return;
    }
    if (!x->op_class->ref()) {
        error("class not found!");
        return;

    }
    
    {
        //x->op_class->ref()->class_name = a.asString();

        char c1[] = "#00C0FF";
        x->e_box.b_boxparameters.d_bordercolor = hex_to_rgba(c1);

        canvas_setcurrent(x->parent_canvas);

        x->instance = new OPInstance(x->op_class->ref());
        
        if (x->instance) {
            x->instance->addInstanceOut(x->out1);

            x->buffer = x->instance->getBufferFor(gensym("out1"), x->vec_size);
        }

        ebox_invalidate_layer((t_ebox*)x, gensym("background_layer"));
        ebox_redraw((t_ebox*)x);
    }
}

static void exp_instance_freeinstance(t_exp_instance* x, t_symbol* id, int argc, t_atom* argv)
{

    exp_instance_delete(x);

    x->e_box.b_boxparameters.d_bordercolor = rgba_red;

    ebox_invalidate_layer((t_ebox*)x, gensym("background_layer"));
    ebox_redraw((t_ebox*)x);
}

static void exp_instance_update(t_exp_instance* x, t_symbol* s, int argc, t_atom* argv)
{
    std::string className = x->instance->class_name;

    exp_instance_freeinstance(x, 0, 0, 0);
    exp_instance_newinstance(x, 0, 1, AtomList(Atom(gensym(className.c_str()))).toPdData());
}
#pragma mark -

static void exp_instance_setobject(t_exp_instance* x, t_symbol* s, int argc, t_atom* argv)
{
    if (argc < 1) {

        exp_instance_freeinstance(x, 0, 0, 0);

    } else {
        if (x->instance)
            if (x->instance->canvas) {

                canvas_vis(x->instance->canvas, 0);

                exp_instance_freeinstance(x, 0, 0, 0);
            }

        Atom a = argv[0];
        //postatom(argc, argv); post("");
        x->instance = OPInstance::findBySymbol(a.asSymbol());

        if (x->instance) {
            x->instance->retain();
            x->instance->addInstanceOut(x->out1);

            char c1[] = "#00C0FF";
            x->e_box.b_boxparameters.d_bordercolor = hex_to_rgba(c1);

            ebox_invalidate_layer((t_ebox*)x, gensym("background_layer"));
            ebox_redraw((t_ebox*)x);

            if (!x->instance->canvas) {
                x->e_box.b_boxparameters.d_bordercolor = rgba_red;

                ebox_invalidate_layer((t_ebox*)x, gensym("background_layer"));
                ebox_redraw((t_ebox*)x);
            }

            x->buffer = x->instance->getBufferFor(gensym("out1"), x->vec_size);

        } else {
            error("instance not found!");

            x->e_box.b_boxparameters.d_bordercolor = rgba_red;

            ebox_invalidate_layer((t_ebox*)x, gensym("background_layer"));
            ebox_redraw((t_ebox*)x);
        }
    }
}

static void exp_instance_getobject(t_exp_instance* x, t_symbol* s, int argc, t_atom* argv)
{
    if (x->op_class->ref()) {
        //x->instance->canvas = x->instance->canvas;
        x->instance->class_name = x->op_class->ref()->class_name;
    }

    Atom a;

    AtomList list;

    a = Atom(gensym("setobject"));
    list.append(a);
    a = Atom(x->instance->symbol);
    list.append(a);

    list.outputAsAny(x->out1);
}

#pragma mark -
static void exp_instance_getproperty(t_exp_instance* x, t_symbol* id, int argc, t_atom* argv)
{
    if (argc < 1)
        return;

    Atom a = argv[0];
    if (!a.isSymbol()) {
        error("bad property name");
        return;
    }

    AtomList list = AtomList(argc, argv);
    if (x->instance)
        x->instance->callGetter(list);
}

static void exp_instance_setproperty(t_exp_instance* x, t_symbol* id, int argc, t_atom* argv)
{
    if (argc < 1)
        return;

    Atom a = argv[0];
    if (!a.isSymbol()) {
        error("bad property name");
        return;
    }

    AtomList list = AtomList(argc, argv);
    if (x->instance)
        x->instance->callSetter(list);
}

#pragma mark -

//t_int *exp_instance_perform(t_int *w)
//{
//    //t_exp_instance *x = (t_exp_instance *)(w[1]);
//    t_sample  *in1 =    (t_sample *)(w[2]);
//    t_sample  *out =    (t_sample *)(w[3]);
//    int          n =           (int)(w[4]);
//
//    while (n--) *out++ = (*in1++);
//
//    return (w+6);
//}
//static void exp_instance_dsp(t_exp_instance* x, t_signal **sp)
//{
//
//    dsp_add(exp_instance_perform, 4, x,
//            sp[0]->s_vec, sp[1]->s_vec, sp[0]->s_n);
//}

//ui_scope_perform(ui_scope* x, t_object*,
//                 t_sample** ins, long,
//                 t_sample**, long,
//                 long sampleframes, long, void*)

static void exp_instance_perform(t_exp_instance* x, t_object*,
    t_sample** ins, long,
    t_sample** , long,
    long sampleframes, long, void*)
{

        t_sample* in = ins[0];
    //    t_sample* out;

    if (x->buffer)
        std::copy(x->buffer, x->buffer +  64, in);
    
    //TODO check sampleframes!

    //
    //        std::copy(outs[0], outs[0] + sampleframes, x->buffer);
}

static void exp_instance_dsp(t_exp_instance* x, t_object* dsp, short* /*count*/, double /*samplerate*/, long vec_size /*maxvectorsize*/, long /*flags*/)
{
    //x->vec_size = (int)vec_size;

    ((t_edsp*)dsp)->d_flags = E_NO_INPLACE;
    object_method(dsp, gensym("dsp_add"), x, reinterpret_cast<method>(exp_instance_perform), 0, NULL);
}

#pragma mark -

static void exp_instance_any(t_exp_instance* x, t_symbol* s, int argc, t_atom* argv)
{
    if (x->instance) {
        AtomList list = Atom(s);
        list.append(AtomList(argc, argv));

        x->instance->callMethod(list);
    }
}

static void exp_instance_methodlist(t_exp_instance* x, t_symbol*, int argc, t_atom* argv)
{
    if (x->instance) {
        AtomList list = x->instance->getMethodList();

        post("Methods:");
        postatom((int)list.size(), list.toPdData());
        post("");
        list = x->instance->getPropertyList();
        post("Properties:");
        postatom((int)list.size(), list.toPdData());
        post("");

        list = x->instance->getDynamicMethodList();
        post("Dynamically added Methods:");
        postatom((int)list.size(), list.toPdData());
        post("");
    }
}

static void exp_instance_refcount(t_exp_instance* x, t_symbol*, int argc, t_atom* argv)
{
    if (x->instance) {
        post("reference count: %i", x->instance->getRefCount());
    }
}

#pragma mark -
#pragma mark object box
static void* exp_instance_new(t_symbol* id, int argc, t_atom* argv)
{

    t_exp_instance* x = reinterpret_cast<t_exp_instance*>(eobj_new(exp_instance_class));

    x->parent_canvas = canvas_getcurrent();

    x->op_class = new OPClasses("", OBJ_NAME);

    ebox_new((t_ebox*)x, 0);
    t_binbuf* d = binbuf_via_atoms(argc, argv);

    x->txt = etext_layout_create();
    x->fnt = efont_create(gensym("Monaco"), gensym("normal"), gensym(""), 10);

    canvas_setcurrent(x->parent_canvas);

    if (x && d) {
        ebox_attrprocess_viabinbuf(x, d);
    }

    ebox_ready((t_ebox*)x);

    x->e_box.b_boxparameters.d_boxfillcolor = rgba_greylight;
    x->e_box.b_boxparameters.d_bordercolor = rgba_red;

    x->out1 = outlet_new((t_object*)x, &s_anything);
    x->out2 = outlet_new((t_object*)x, &s_signal);

    x->in2 = inlet_new(&x->e_box.b_obj.o_obj, &x->e_box.b_obj.o_obj.ob_pd, &s_signal, &s_signal);

    x->vec_size = 64; //test
    x->instance = 0;

    return static_cast<void*>(x);
}

static void exp_instance_free(t_exp_instance* x)
{
    exp_instance_delete(x);
}

static void exp_instance_vis(t_exp_instance* x, t_symbol*, int argc, t_atom* argv)
{
    if (argc < 1)
        return;
    Atom a = argv[0];

    if (!a.isFloat())
        return;

    if (x->instance->canvas) {
        //post("vis");
        canvas_vis(x->instance->canvas, (a.asInt() > 0));
    }
}

static void exp_instance_paint(t_object* z, t_object* view)
{
    t_symbol* bgl = gensym("background_layer");
    t_rect rect;
    ebox_get_rect_for_view((t_ebox*)z, &rect);

    t_exp_instance* zx = (t_exp_instance*)z;

    t_elayer* g = ebox_start_layer((t_ebox*)z, bgl, rect.width, rect.height);

    if (g) {
        if (zx->instance) {

            std::string disp_name = (zx->instance) ? zx->instance->class_name : "—";

            etext_layout_set(zx->txt, disp_name.c_str(), zx->fnt, 2, 15, rect.width, rect.height / 2, ETEXT_DOWN_LEFT, ETEXT_JLEFT, ETEXT_NOWRAP);
            etext_layout_draw(zx->txt, g);
        }
    }

    ebox_end_layer((t_ebox*)z, bgl);
    ebox_paint_layer((t_ebox*)z, bgl, 0., 0.);
}

static void exp_instance_oksize(t_object* z, t_rect* newrect)
{
    newrect->width = 90;
    newrect->height = 15;
}

static void exp_instance_click(t_exp_instance* x, t_symbol* s, int argc, t_atom* argv)
{
    if (x->instance)
        if (x->instance->canvas)
            canvas_vis(x->instance->canvas, 1);
}

extern "C" void setup_exp0x2einstance()
{

    exp_instance_class = eclass_new((OBJ_NAME),
        reinterpret_cast<t_typ_method>(exp_instance_new),
        reinterpret_cast<t_typ_method>(0),
        sizeof(t_exp_instance), CLASS_PATCHABLE, A_GIMME, 0);

    eclass_dspinit(exp_instance_class);
    eclass_guiinit(exp_instance_class, 0);

    CLASS_ATTR_DEFAULT(exp_instance_class, "size", 0, "90. 15.");

    //hide standard CICM attributes
    CLASS_ATTR_INVISIBLE(exp_instance_class, "fontname", 1);
    CLASS_ATTR_INVISIBLE(exp_instance_class, "fontweight", 1);
    CLASS_ATTR_INVISIBLE(exp_instance_class, "fontslant", 1);
    CLASS_ATTR_INVISIBLE(exp_instance_class, "fontsize", 1);

    // background / border color
    CLASS_ATTR_LABEL(exp_instance_class, "bgcolor", 0, "Background Color");
    CLASS_ATTR_ORDER(exp_instance_class, "bgcolor", 0, "1");
    CLASS_ATTR_DEFAULT_SAVE_PAINT(exp_instance_class, "bgcolor", 0, "0.75 0.75 0.75 1.");
    CLASS_ATTR_STYLE(exp_instance_class, "bgcolor", 0, "color");

    eclass_addmethod(exp_instance_class, (t_typ_method)(exp_instance_update), ("update"), A_NULL, 0);

    eclass_addmethod(exp_instance_class, (t_typ_method)(exp_instance_newinstance), ("new"), A_GIMME, 0);
    eclass_addmethod(exp_instance_class, (t_typ_method)(exp_instance_freeinstance), ("free"), A_GIMME, 0);

    eclass_addmethod(exp_instance_class, (t_typ_method)exp_instance_methodlist, ("methodlist"), A_NULL, 0);
    eclass_addmethod(exp_instance_class, (t_typ_method)exp_instance_refcount, ("refcount"), A_NULL, 0);

    eclass_addmethod(exp_instance_class, (t_typ_method)(exp_instance_getobject), ("getobject"), A_GIMME, 0);
    eclass_addmethod(exp_instance_class, (t_typ_method)(exp_instance_setobject), ("setobject"), A_GIMME, 0);

    //todo move to 'anything', use @prop / @prop?
    eclass_addmethod(exp_instance_class, (t_typ_method)(exp_instance_getproperty), ("getp"), A_GIMME, 0);
    eclass_addmethod(exp_instance_class, (t_typ_method)(exp_instance_setproperty), ("setp"), A_GIMME, 0);

    eclass_addmethod(exp_instance_class, (t_typ_method)(exp_instance_any), ("anything"), A_GIMME, 0);

    eclass_addmethod(exp_instance_class, (method)(exp_instance_paint), ("paint"), A_NULL, 0);
    eclass_addmethod(exp_instance_class, (method)(exp_instance_oksize), ("oksize"), A_GIMME, 0);

    eclass_addmethod(exp_instance_class, (t_typ_method)exp_instance_click, ("mousedown"), A_NULL, 0);

    //audio
    //eclass_addmethod(exp_instance_class, (t_typ_method)exp_instance_dsp, ("dsp"), A_NULL, 0);

    eclass_register(CLASS_BOX, exp_instance_class);

    //CLASS_MAINSIGNALIN((t_class*)exp_instance_class, t_exp_instance, in_f);
}

#include "oop_common.h"

t_eclass* exp_instance_class;

struct t_exp_instance {
    t_ebox e_box;
    
    t_canvas *parent_canvas;    //patch
    
    t_canvas *local_canvas;     //instance
    
    oPDClass *global;           //class
    std::string class_name;
    
    oPDInstance *instance;
    
    t_etext *txt;
    t_efont *fnt;
    
    
    t_outlet *out1;
};

#define OBJ_NAME "exp.instance"

using namespace ceammc;

static void exp_instance_delete(t_exp_instance* x)
{
    if (x->local_canvas)
    {
//        if (x->local_canvas->gl_list)
//        {
//            glist_delete(x->local_canvas, x->local_canvas->gl_list);
//        }
        
        
        //todo free
        x->instance->ref().freeInstanceOut(x->out1);
        x->local_canvas = 0;
    }
}

static void exp_instance_update(t_exp_instance* x, t_symbol*s, int argc, t_atom* argv)
{
    
    if (x->global->ref())
    {
        
        // create instance
        canvas_setcurrent(x->parent_canvas);
        
        if (!x->local_canvas)
        {
            //glist_init(x->local_canvas);
            x->local_canvas = (t_canvas*)subcanvas_new(gensym(x->class_name.c_str())); //LISP lol
            x->local_canvas->gl_havewindow = 1;
            x->local_canvas->gl_env = 0;
            
            //x->instance->ref() = x->local_canvas;
            
        }
        else
        {
            //cleanup
            if (x->local_canvas->gl_list)
            {
                glist_delete(x->local_canvas, x->local_canvas->gl_list);
            }
        }
        
        t_canvas *src_canvas = x->global->ref();
        
        t_binbuf *b1 = binbuf_new();
        
        canvas_saveto(src_canvas, b1);
        
        int blen=0;
        char *bchar;
        binbuf_gettext(b1, &bchar, &blen);
        
        
        //int natoms = binbuf_getnatom(b1);
        //t_atom* vec = binbuf_getvec(b1);
        //postatom(natoms, vec);
        
//        AtomList list;
//        list.append(Atom(gensym("0")));
//        list.append(Atom(gensym("0")));
//        list.append(Atom(gensym("cnv")));
//        list.append(Atom(gensym("5")));
//        list.append(Atom(gensym("300")));
//        list.append(Atom(gensym("150")));
//        list.append(Atom(gensym("empty")));
//        list.append(Atom(gensym("empty")));
//        list.append(Atom(gensym("Instance")));
//        list.append(Atom(gensym("0")));
//        list.append(Atom(gensym("18")));
//        list.append(Atom(gensym("0")));
//        list.append(Atom(gensym("24")));
//        list.append(Atom(gensym("0")));
//        list.append(Atom(gensym("0")));
//        list.append(Atom(gensym("0")));
//        pd_typedmess((t_pd*)x->local_canvas, gensym("obj"), (int)list.size(), list.toPdData());
        
        canvas_dopaste(x->local_canvas, b1);
        canvas_vis(x->local_canvas, 0);
        canvas_setcurrent(x->parent_canvas);
        
    }
}

static void exp_instance_setobject(t_exp_instance* x, t_symbol*s, int argc, t_atom* argv)
{
    if (argc<1)
    {
        x->class_name = "";
        
        //todo free!
        x->instance->ref().freeInstanceOut(x->out1);
        x->local_canvas = 0;
        
        x->e_box.b_boxparameters.d_bordercolor = rgba_red;
        
        ebox_invalidate_layer((t_ebox *)x, gensym("background_layer"));
        ebox_redraw((t_ebox *)x);

        
    }
    else
    {
        if (x->local_canvas)
            {canvas_vis(x->local_canvas, 0);}
        
        Atom a = argv[0];
        
        x->instance = new oPDInstance(a.asString(), OBJ_NAME);
        
        x->local_canvas = x->instance->ref().canvas;
        x->class_name = x->instance->ref().class_name;
        x->instance->ref().addInstanceOut(x->out1);
        
        char c1[] = "#00C0FF";
        x->e_box.b_boxparameters.d_bordercolor = hex_to_rgba(c1);
        
        ebox_invalidate_layer((t_ebox *)x, gensym("background_layer"));
        ebox_redraw((t_ebox *)x);
        
        if (!x->local_canvas)
        {
            x->e_box.b_boxparameters.d_bordercolor = rgba_red;
            
            ebox_invalidate_layer((t_ebox *)x, gensym("background_layer"));
            ebox_redraw((t_ebox *)x);
            
            
        }
        
    }
    
}

static void exp_instance_getobject(t_exp_instance* x, t_symbol*s, int argc, t_atom* argv)
{
    
    x->instance->ref().canvas = x->local_canvas;
    x->instance->ref().class_name = x->class_name;
    
    Atom a;
    
    AtomList list;
    
    a= Atom(gensym("setobject"));
    list.append(a);
    a= Atom(gensym(to_string(x->local_canvas).c_str()));
    list.append(a);
    
    list.outputAsAny(x->out1);
}

#pragma mark -
static void exp_instance_getproperty(t_exp_instance* x, t_symbol*id, int argc, t_atom* argv)
{
    if (argc<1) return;
    
    Atom a = argv[0];
    if (!a.isSymbol()) {error("bad property name"); return;}
    
    AtomList list = AtomList(argc, argv);
    x->instance->ref().callGetter(list);
    
}

static void exp_instance_setproperty(t_exp_instance* x, t_symbol*id, int argc, t_atom* argv)
{
    if (argc<1) return;
    
    Atom a = argv[0];
    if (!a.isSymbol()) {error("bad property name"); return;}
    
    AtomList list = AtomList(argc, argv);
    x->instance->ref().callSetter(list);
    
}


#pragma mark -

static void exp_instance_setclass(t_exp_instance* x, t_symbol*id, int argc, t_atom* argv)
{
    if (argc<1)
    {
        error("no class name provided!");
        x->e_box.b_boxparameters.d_bordercolor = rgba_red;
        exp_instance_delete(x);
        
        ebox_invalidate_layer((t_ebox *)x, gensym("background_layer"));
        ebox_redraw((t_ebox *)x);
    }
    Atom a = argv[0];
    if (!a.isSymbol())
    {
        error("bad class name!");
        x->e_box.b_boxparameters.d_bordercolor = rgba_red;
        exp_instance_delete(x);
        
        ebox_invalidate_layer((t_ebox *)x, gensym("background_layer"));
        ebox_redraw((t_ebox *)x);
    }
    
    x->class_name = a.asString();
    
    x->global = new oPDClass(a.asString(), OBJ_NAME);
    
    
    if (!x->global->ref())
    {
        error("class not found!");
        x->e_box.b_boxparameters.d_bordercolor = rgba_red;
        
        ebox_invalidate_layer((t_ebox *)x, gensym("background_layer"));
        ebox_redraw((t_ebox *)x);
    }
    else
    {
        //printf("update %s\n", x->class_name.c_str());
        exp_instance_update(x, 0, 0, 0);
        char c1[] = "#00C0FF";
        x->e_box.b_boxparameters.d_bordercolor = hex_to_rgba(c1);
        
        ebox_invalidate_layer((t_ebox *)x, gensym("background_layer"));
        ebox_redraw((t_ebox *)x);
        
        std::string str = to_string(x->local_canvas);
        x->instance = new oPDInstance(str, OBJ_NAME);
        //x->instance->ref().inst_out = x->out1;
        x->instance->ref().addInstanceOut(x->out1);
        
        
    }
    
    
}

static void* exp_instance_new(t_symbol *id, int argc, t_atom *argv)
{
    
    t_exp_instance* x = reinterpret_cast<t_exp_instance*>(eobj_new(exp_instance_class));
    
    x->parent_canvas = canvas_getcurrent();
    
    std::string str = to_string(x->local_canvas);
    x->instance = new oPDInstance(str, OBJ_NAME);
    x->instance->ref().addInstanceOut(x->out1);
    
    
    ebox_new((t_ebox *)x, 0 );
    t_binbuf* d = binbuf_via_atoms(argc,argv);
    
    
    x->class_name = "";
    x->global = new oPDClass("", OBJ_NAME);
    
    
    x->txt = etext_layout_create();
    x->fnt = efont_create(gensym("Monaco"), gensym("normal"), gensym(""), 10);
    
    x->local_canvas = 0;
    
    canvas_setcurrent(x->parent_canvas);
    
    if (x && d)
    {
        ebox_attrprocess_viabinbuf(x, d);
        
    }
    
    ebox_ready((t_ebox *)x);
    
    x->e_box.b_boxparameters.d_boxfillcolor = rgba_greylight;
    x->e_box.b_boxparameters.d_bordercolor = rgba_red;
    
    x->out1 = outlet_new((t_object*)x, &s_anything);
    
    return static_cast<void*>(x);
    
    
}

static void exp_instance_vis(t_exp_instance* x, t_symbol*, int argc, t_atom* argv)
{
    if (argc<1) return;
    Atom a = argv[0];
    
    if (!a.isFloat()) return;
    
    if (x->local_canvas)
    {
        //post("vis");
        canvas_vis(x->local_canvas, (a.asInt()>0));
    }
}

static void exp_instance_any(t_exp_instance* x, t_symbol*s, int argc, t_atom* argv)
{
    //if (argc<1) return;
    AtomList list = Atom(s);
    list.append(AtomList(argc,argv));
    
    x->instance->ref().callMethod(list);
    
}

#pragma mark -

static void exp_instance_paint(t_object *z, t_object *view)
{
    t_symbol *bgl = gensym("background_layer");
    //float size;
    t_rect rect;
    ebox_get_rect_for_view((t_ebox *)z, &rect);
    
    t_exp_instance *zx = (t_exp_instance*)z;
    
    t_elayer *g = ebox_start_layer((t_ebox *)z, bgl, rect.width, rect.height);
    
    
    if(g)
    {
        
        //printf("paint %f %f\n", rect.width, rect.height);
        
        etext_layout_set(zx->txt, zx->class_name.c_str(), zx->fnt, 2, 15, rect.width, rect.height/2, ETEXT_DOWN_LEFT, ETEXT_JLEFT, ETEXT_NOWRAP);
        etext_layout_draw(zx->txt, g);
        
    }
    
    ebox_end_layer((t_ebox*)z, bgl);
    ebox_paint_layer((t_ebox *)z, bgl, 0., 0.);
}


static void exp_instance_free(t_exp_instance* x)
{
    canvas_free(x->local_canvas);
    
}

static void exp_instance_oksize(t_object *z, t_rect *newrect)
{
    newrect->width = 90;
    newrect->height = 15;
}

static void exp_instance_dblclick(t_exp_instance* x, t_symbol* s, int argc, t_atom* argv)
{
    exp_instance_vis(x, 0, 1, AtomList(1, Atom(1)).toPdData());
}


extern "C" void setup_exp0x2einstance()
{
    
    exp_instance_class = eclass_new((OBJ_NAME),
                                    reinterpret_cast<t_typ_method>(exp_instance_new),
                                    reinterpret_cast<t_typ_method>(0),
                                    sizeof(t_exp_instance), CLASS_PATCHABLE, A_GIMME,0);
    
    eclass_guiinit(exp_instance_class, 0);
    
    CLASS_ATTR_DEFAULT (exp_instance_class, "size", 0, "90. 15.");
    
    //hide standard CICM attributes
    CLASS_ATTR_INVISIBLE            (exp_instance_class, "fontname", 1);
    CLASS_ATTR_INVISIBLE            (exp_instance_class, "fontweight", 1);
    CLASS_ATTR_INVISIBLE            (exp_instance_class, "fontslant", 1);
    CLASS_ATTR_INVISIBLE            (exp_instance_class, "fontsize", 1);
    
    // background / border color
    CLASS_ATTR_LABEL                (exp_instance_class, "bgcolor", 0, "Background Color");
    CLASS_ATTR_ORDER                (exp_instance_class, "bgcolor", 0, "1");
    CLASS_ATTR_DEFAULT_SAVE_PAINT   (exp_instance_class, "bgcolor", 0, "0.75 0.75 0.75 1.");
    CLASS_ATTR_STYLE                (exp_instance_class, "bgcolor", 0, "color");
    
    eclass_addmethod(exp_instance_class,(t_typ_method)(exp_instance_vis), ("vis"), A_GIMME,0);
    
    eclass_addmethod(exp_instance_class,(t_typ_method)(exp_instance_update), ("update"), A_NULL,0);
    
    eclass_addmethod(exp_instance_class,(t_typ_method)(exp_instance_setclass), ("class"), A_GIMME,0);   //will remove
    eclass_addmethod(exp_instance_class,(t_typ_method)(exp_instance_setclass), ("new"), A_GIMME,0);
    
    //eclass_addmethod(exp_instance_class,(t_typ_method)(exp_instance_setclass), ("free"), A_GIMME,0);
    
    eclass_addmethod(exp_instance_class,(t_typ_method)(exp_instance_getobject), ("getobject"), A_GIMME,0);
    eclass_addmethod(exp_instance_class,(t_typ_method)(exp_instance_setobject), ("setobject"), A_GIMME,0);
    
    //todo move to 'anything', use @prop / @prop?
    eclass_addmethod(exp_instance_class,(t_typ_method)(exp_instance_getproperty), ("getp"), A_GIMME,0);
    eclass_addmethod(exp_instance_class,(t_typ_method)(exp_instance_setproperty), ("setp"), A_GIMME,0);
    
    eclass_addmethod(exp_instance_class, (t_typ_method)(exp_instance_any), ("anything"), A_GIMME, 0);
    
    eclass_addmethod(exp_instance_class, (method)(exp_instance_paint), ("paint"), A_NULL,0);
    eclass_addmethod(exp_instance_class, (method)(exp_instance_oksize), ("oksize"), A_GIMME,0);
    
    eclass_addmethod(exp_instance_class, (t_typ_method)exp_instance_dblclick, ("dblclick"), A_NULL, 0);
    
    
    
    eclass_register(CLASS_BOX, exp_instance_class);
    
}

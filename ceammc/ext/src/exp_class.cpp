#include "oop_common.h"

t_eclass* exp_class_class;


struct t_exp_class {
    t_ebox e_box;
    
    t_canvas *parent_canvas;
    t_canvas *sub_canvas;
    
    OPClass *op_class;
    
    t_atom *patch_a;
    int patch_c;
    
    t_outlet *out1;
    
};

#define OBJ_NAME "exp.class"

using namespace ceammc;

static void exp_class_read(t_exp_class* x, t_symbol*, int argc, t_atom* argv)
{
    if (argc<1) {error("specify file name"); return;}
    
    Atom a = Atom(argv[0]);
    x->op_class->ref()->readFile(a.asString(), x->parent_canvas);
    
}

static void exp_class_write(t_exp_class* x, t_symbol*, int argc, t_atom* argv)
{
    if (argc<1) {error("specify file name"); return;}
    
    Atom a = Atom(argv[0]);
    x->op_class->ref()->writeFile(a.asString(), x->parent_canvas);

}

#pragma mark -

static void exp_class_click(t_exp_class* x, t_symbol*, int argc, t_atom* argv)
{
    canvas_vis(x->sub_canvas, 1);
}


static void* exp_class_new(t_symbol *id, int argc, t_atom *argv)
{
    if (argc<1)
    {
        error("no class name provided!"); return 0;
    }
    
    Atom a = argv[0];
    if (!a.isSymbol())
    {
        error("bad class name!"); return 0;
    }
    
    t_exp_class* x = reinterpret_cast<t_exp_class*>(eobj_new(exp_class_class));
    
    x->patch_a = (t_atom*)malloc(0);
    x->patch_c = 0;
    
    //x->op_class->class_name = a.asSymbol();
    
    //ebox_new((t_ebox *)x, 0 );
    t_binbuf* d = binbuf_via_atoms(argc,argv);
    if (x && d)
    {
        ebox_attrprocess_viabinbuf(x, d);
        
        //        if (x->op_class->class_name)
        //            poststring(x->op_class->class_name->s_name);
    }
    
    x->parent_canvas = canvas_getcurrent();
    
    
    //should be read only for others
    x->op_class = new OPClass(a.asString(), OBJ_NAME);
    
    if (!x->op_class->ref())
    {
        x->op_class->ref() = new t_op_class;
        
        x->sub_canvas = (t_canvas*)subcanvas_new(gensym(x->op_class->ref()->class_name.c_str()));
        x->sub_canvas->gl_havewindow = 1;
        x->sub_canvas->gl_isclone = 1;
        
        canvas_vis(x->sub_canvas, 0);
        
        x->op_class->ref()->canvas = x->sub_canvas;
        
        x->op_class->ref()->class_name = a.asString();
        
        
        // loader - old
        //        t_binbuf *b1= binbuf_new();
        //        binbuf_add(b1, x->patch_c, x->patch_a);
        //        int natoms = binbuf_getnatom(b1);
        //        t_atom* vec = binbuf_getvec(b1);
        //        postatom(natoms, vec);
        //
        //        canvas_dopaste(x->sub_canvas, b1);
        //        canvas_setcurrent(x->parent_canvas);
        
        
        //attempt to load file
        std::string class_name = a.asString() + ".class.pd";
        Atom a_class_name = Atom(gensym(class_name.c_str()));
        exp_class_read(x, 0, 1, AtomList(a_class_name).toPdData());
        
        
    }
    else
    {
        x->sub_canvas = x->op_class->ref()->canvas;
    }
    
    //TODO multiple class save handling
    
    x->out1 = outlet_new((t_object*)x, &s_anything);
    ebox_ready((t_ebox *)x);
    
    return static_cast<void*>(x);
    
    
}


static void exp_class_free(t_exp_class* x)
{
    if (x->op_class->ref())
    {
        if (x->sub_canvas)
        {
            std::string class_name = x->op_class->ref()->class_name + ".class.pd";
            Atom a_class_name = Atom(gensym(class_name.c_str()));
            exp_class_write(x, 0, 1, AtomList(a_class_name).toPdData());
            
            canvas_free(x->sub_canvas);
            
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
                                 sizeof(t_exp_class), 0, A_GIMME,0);
    
    //eclass_addmethod(exp_class_class,(t_typ_method)(exp_class_vis), ("vis"), A_GIMME,0);
    
    eclass_addmethod(exp_class_class, (t_typ_method)(exp_class_read), ("readclass"), A_GIMME, 0);
    eclass_addmethod(exp_class_class, (t_typ_method)(exp_class_write), ("writeclass"), A_GIMME, 0);
    
    eclass_addmethod(exp_class_class, (t_typ_method)exp_class_click, ("click"), A_NULL, 0);
    
    //    eclass_addmethod(exp_class_class,(t_typ_method)(exp_class_update), ("update"), A_NULL,0);
    //    eclass_addmethod(exp_class_class, (t_typ_method)(exp_class_inlet_proxy), ("anything"), A_GIMME, 0);

    //todo
//    CLASS_ATTR_SYMBOL(exp_class_class, "class_name", 0, t_exp_class, class_name);
//    CLASS_ATTR_DEFAULT_SAVE_PAINT   (exp_class_class, "class_name", 0, "[class]");
    
    
    
    eclass_register(CLASS_BOX, exp_class_class);
    
    
    
    
}

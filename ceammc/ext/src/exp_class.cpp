#include "oop_common.h"

t_eclass* exp_class_class;


struct t_exp_class {
    //t_object x_obj;
    
    t_ebox e_box;
    
    t_canvas *parent_canvas;
    t_canvas *sub_canvas;
    
    t_symbol* class_name;
    
    
    OPClass *global;
    
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
    char * filename = (char*)a.asString().c_str();
    
    t_binbuf *b;
    b = binbuf_new();
    
    binbuf_read_via_canvas(b,filename, x->parent_canvas, 0);
    
    // to canvas
    // find better way to load?
    
    if (x->sub_canvas->gl_list)
    {
        glist_delete(x->sub_canvas, x->sub_canvas->gl_list);
    }
    
    int blen=0;
    char *bchar;
    binbuf_gettext(b, &bchar, &blen);
    
    //    int natoms = binbuf_getnatom(b);
    //    t_atom* vec = binbuf_getvec(b);
    
    //postatom(natoms, vec);
    
    canvas_paste_class(x->sub_canvas, b);
    canvas_vis(x->sub_canvas, 0);
    canvas_setcurrent(x->parent_canvas);
    
}

static void exp_class_write(t_exp_class* x, t_symbol*, int argc, t_atom* argv)
{
    if (argc<1) {error("specify file name"); return;}
    
    //postatom(argc, argv);
    
    Atom a = Atom(argv[0]);
    char * filename = (char*)a.asString().c_str();
    
    t_binbuf *b = binbuf_new();
    
    canvas_saveto(x->sub_canvas, b);
    
    const char * dir = canvas_getdir(x->parent_canvas)->s_name;
    binbuf_write(b, filename, (char *)dir, 0);
    
    //post("wrote %s to %s", filename, dir );
    post("saved class: %s ", filename);
    //postatom(argc, argv);
}

#pragma mark -

//static void exp_class_vis(t_exp_class* x, t_symbol*, int argc, t_atom* argv)
//{
//    if (argc<1) return;
//    Atom a = argv[0];
//    
//    if (!a.isFloat()) return;
//    
//    if (x->sub_canvas)
//    {
//        //post("vis");
//        canvas_vis(x->sub_canvas, (a.asInt()>0));
//    }
//}


static void exp_class_click(t_exp_class* x, t_symbol*, int argc, t_atom* argv)
{
    //exp_class_vis(x, 0, 1, AtomList(1, Atom(1)).toPdData());
    
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
    
    x->class_name = a.asSymbol();
    
    //ebox_new((t_ebox *)x, 0 );
    t_binbuf* d = binbuf_via_atoms(argc,argv);
    if (x && d)
    {
        ebox_attrprocess_viabinbuf(x, d);
        
        //        if (x->class_name)
        //            poststring(x->class_name->s_name);
    }
    
    x->parent_canvas = canvas_getcurrent();
    
    
    //should be read only for others
    x->global = new OPClass(a.asString(), OBJ_NAME);
    
    if (!x->global->ref())
    {
        x->sub_canvas = (t_canvas*)subcanvas_new((x->class_name));
        x->sub_canvas->gl_havewindow = 1;
        x->sub_canvas->gl_isclone = 1;
        
        canvas_vis(x->sub_canvas, 0);
        
        x->global->ref() = x->sub_canvas;
        
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
        x->sub_canvas = x->global->ref();
    }
    
    
    
    //TODO multiple class save handling
    
    
    
    
    
    
    
    //
    
    x->out1 = outlet_new((t_object*)x, &s_anything);
    ebox_ready((t_ebox *)x);
    
//    exp_class_class->c_class.c_wb->w_activatefn = (t_activatefn)(exp_class_dblclick);
    
    //if(c->c_widget.w_dblclick)
    
    
    //sys_vgui("bind %s <Double-Button-1> {+pdsend {%s dblclick %%x %%y %%s}}\n", x->e_box.b_obj.o_obj.te_g, x->e_box.b_obj.o_id->s_name);
    
    
    //printf("dosave %lu ->",(long)eobj_getclass(x)->c_widget.w_dosave);
    
    //eobj_getclass(x)->c_class.c_savefn = (t_savefn)exp_class_save;
    
    //printf("dosave %lu\n",(long)exp_class_save);
    
    return static_cast<void*>(x);
    
    
}


static void exp_class_free(t_exp_class* x)
{
    if (x->global->ref())
    {
        if (x->sub_canvas)
        {
            std::string class_name = std::string(x->class_name->s_name) + ".class.pd";
            Atom a_class_name = Atom(gensym(class_name.c_str()));
            exp_class_write(x, 0, 1, AtomList(a_class_name).toPdData());
            
            canvas_free(x->sub_canvas);
            
            x->global->ref() = 0;
        }
        
        delete x->global;
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
    
    
    CLASS_ATTR_SYMBOL(exp_class_class, "class_name", 0, t_exp_class, class_name);
    CLASS_ATTR_DEFAULT_SAVE_PAINT   (exp_class_class, "class_name", 0, "[class]");
    
    
    
    eclass_register(CLASS_BOX, exp_class_class);
    
    
    
    
}

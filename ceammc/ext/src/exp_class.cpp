#include "oop_common.h"

t_eclass* exp_class_class;


struct t_exp_class {
    //t_object x_obj;
    
    t_ebox e_box;
    
    t_canvas *parent_canvas;
    t_canvas *sub_canvas;
    
    t_symbol* class_name;
    
    
    oPDClass *global;
    
    t_atom *patch_a;
    int patch_c;
    
    t_outlet *out1;
    
};

#define OBJ_NAME "exp.class"

using namespace ceammc;

void exp_class_save(t_ebox* x, t_binbuf *b)
{
    post("ext_save");
//    t_exp_class *zx = (t_exp_class*)x;

    //
//    t_binbuf *b1 = binbuf_new();
//    canvas_saveto(zx->sub_canvas, b1);
//    int blen=0;
//    char *bchar;
//    binbuf_gettext(b1, &bchar, &blen);
//    
//    zx->patch_c = binbuf_getnatom(b1);
//    zx->patch_a = binbuf_getvec(b1);
//    
////    post("data");
////    poststring(bchar);
//    
//    //ebox_attrprocess_viabinbuf(x, b1);
//    //ebox_attrprint(x);
//    
//    //CLASS_ATTR_DEFAULT(exp_class_class, "class_patch", 0, bchar);
//    //CLASS_ATTR_SAVE(exp_class_class, "class_patch", 0);
//    
//    
//    ebox_attrceammcprint(x);
//    
//    
//    t_eclass *c = eobj_getclass(x);
//    printf("name\n");
//    poststring(c->c_attr[0]->name->s_name);
//    printf("\n---");
//    
//    printf("src %d", zx->patch_c);
//    postatom(zx->patch_c,zx->patch_a);
//    printf("\n---");
//    
////    eclass_attr_setter((t_object*)x, gensym("class_patch"), zx->patch_c, zx->patch_a);
//    
//    eobj_attr_setvalueof((t_object*)x, gensym("class_patch"), zx->patch_c, zx->patch_a);
//    
//    
//    
//    int argc; t_atom *argv = (t_atom*)malloc(0);
    
    //eclass_attr_getter((t_object*)x, gensym("class_patch"), &argc, &argv);
    
//    postatom(argc,argv);
//    post("---");
//    
//    eobj_attr_getvalueof(x, c->c_attr[0]->name, &argc, &argv);
    
    //eobj_attr_setvalueof
    
//    postatom(argc,argv);
//    printf("---\n");
//    
//    printf("dosave\n");
    
    
    //binbuf_addv(b, "s", zx->class_name.c_str());
    
    //binbuf_add(b, 1, AtomList(Atom(gensym(zx->class_name.c_str()))).toPdData());
    
    
    
    ebox_dosave(x, b);
    
}

static void exp_get_patch(t_exp_class* x, t_symbol*, int &argc, t_atom* &argv)
{
    //printf("getpatch\n");
    
    AtomList list = AtomList(x->patch_c, x->patch_a);
    
    t_exp_class *zx = (t_exp_class*)x;
    
    t_binbuf *b1 = binbuf_new();
    canvas_saveto(zx->sub_canvas, b1);
    int blen=0;
    char *bchar;
    binbuf_gettext(b1, &bchar, &blen);
    
    zx->patch_c = binbuf_getnatom(b1);
    zx->patch_a = binbuf_getvec(b1);
    
    
    //escape
    for (int i=0; i<zx->patch_c; i++)
    {
        if (zx->patch_a[i].a_type == A_SEMI)
        {
            zx->patch_a[i] = *AtomList(Atom(gensym(";"))).toPdData();
        }
        
        if (zx->patch_a[i].a_type == A_COMMA)
        {
            zx->patch_a[i] = *AtomList(Atom(gensym(","))).toPdData();
        }
    }
    
    argc = zx->patch_c;
    argv = zx->patch_a;
    
}

static void exp_set_patch(t_exp_class* x, t_symbol*, int argc, t_atom* argv)
{
    //printf("setpatch\n");
   
    AtomList list = AtomList(argc, argv);
    
    //"unescape"
    for (int i=0; i<list.size(); i++)
    {
        if (list[i].asSymbol()==gensym(";"))
        {
            t_atom a;
            a.a_type = A_SEMI;
            list[i] = a;
        }
        
        if (list[i].asSymbol()==gensym(","))
        {
            t_atom a;
            a.a_type = A_COMMA;
            list[i] = a;
        }
    }
    
    x->patch_c = (int)list.size();
    x->patch_a = list.toPdData();
    

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
    
    
    x->parent_canvas = canvas_getcurrent();
    
    
    //should be read only for others
    x->global = new oPDClass(a.asString(), OBJ_NAME);
    
    if (!x->global->ref())
    {
        x->sub_canvas = (t_canvas*)subcanvas_new((x->class_name));
        x->sub_canvas->gl_havewindow = 1;
        x->sub_canvas->gl_isclone = 1;
        
        x->global->ref() = x->sub_canvas;
        
        canvas_vis(x->sub_canvas, 0);

    }
    else
    {
        x->sub_canvas = x->global->ref();
    }
    
    
    
    
    if (x && d)
    {
        ebox_attrprocess_viabinbuf(x, d);
        
    }
    
    x->out1 = outlet_new((t_object*)x, &s_anything);
    ebox_ready((t_ebox *)x);
    
    
    
    //printf("dosave %lu ->",(long)eobj_getclass(x)->c_widget.w_dosave);
    
    eobj_getclass(x)->c_class.c_savefn = (t_savefn)exp_class_save;
    
    //printf("dosave %lu\n",(long)exp_class_save);
    
    return static_cast<void*>(x);
    
    
}



static void exp_class_vis(t_exp_class* x, t_symbol*, int argc, t_atom* argv)
{
    if (argc<1) return;
    Atom a = argv[0];
    
    if (!a.isFloat()) return;
        
    if (x->sub_canvas)
    {
        //post("vis");
        canvas_vis(x->sub_canvas, (a.asInt()>0));
    }
}



static void exp_class_free(t_exp_class* x)
{
    canvas_free(x->sub_canvas);
    
    delete x->global;
}

extern "C" void setup_exp0x2eclass()
{
    
    exp_class_class = eclass_new((OBJ_NAME),
                                    reinterpret_cast<t_typ_method>(exp_class_new),
                                    reinterpret_cast<t_typ_method>(0),
                                    sizeof(t_exp_class), 0, A_GIMME,0);
    
    eclass_addmethod(exp_class_class,(t_typ_method)(exp_class_vis), ("vis"), A_GIMME,0);
    
//    eclass_addmethod(exp_class_class,(t_typ_method)(exp_class_update), ("update"), A_NULL,0);
//    eclass_addmethod(exp_class_class, (t_typ_method)(exp_class_inlet_proxy), ("anything"), A_GIMME, 0);
    
    
    CLASS_ATTR_INVISIBLE            (exp_class_class, "class_patch", 1);
    
    CLASS_ATTR_ATOM_VARSIZE(exp_class_class, "class_patch", 0, t_exp_class, patch_a, patch_c, 65535);
    CLASS_ATTR_ACCESSORS            (exp_class_class, "class_patch", exp_get_patch, exp_set_patch);
    CLASS_ATTR_ITEMS(exp_class_class,"class_patch", 0, "");
    //CLASS_ATTR_ATOM
    
    CLASS_ATTR_DEFAULT_SAVE_PAINT   (exp_class_class, "class_patch", 0, "");
    
    CLASS_ATTR_SYMBOL(exp_class_class, "class_name", 0, t_exp_class, class_name);
    CLASS_ATTR_DEFAULT_SAVE_PAINT   (exp_class_class, "class_name", 0, "[class]");
    
    
    eclass_register(CLASS_BOX, exp_class_class);
    
    
}

#include "ceammc.h"
#include <m_pd.h>
//#include <g_canvas.h>
#include <stdlib.h>
//
#include "ceammc_atomlist.h"

#include "m_imp.h"

#include <stdio.h>

#include "cicm_wrapper.h"

#include "ceammc_globaldata.h"

t_eclass* exp_instance_class;
typedef ceammc::GlobalData<t_canvas*> oPDClass;

struct t_exp_instance {
    //t_object x_obj;
    
    t_ebox e_box;
    
    t_canvas *parent_canvas;
    
    t_canvas *local_canvas;
    
    oPDClass *global;
    std::string class_name;
    
    //    t_binbuf *patch;
    //
    //    t_canvas *new_canvas;
    //
    //    t_inlet* inlets[64];
    //    int inlet_count;
    //    t_outlet* outlets[64];
    //    int outlet_count;
    
    t_etext *txt;
    t_efont *fnt;
    
};


//*****

//static int canvas_scanbinbuf(int natoms, t_atom *vec, int *p_indexout,
//                             int *p_next)
//{
//    int i, j;
//    int indexwas = *p_next;
//    *p_indexout = indexwas;
//    if (indexwas >= natoms)
//        return (0);
//    for (i = indexwas; i < natoms && vec[i].a_type != A_SEMI; i++)
//        ;
//    if (i >= natoms)
//        *p_next = i;
//    else *p_next = i + 1;
//    return (i - indexwas);
//}
//
//static void canvas_readerror(int natoms, t_atom *vec, int message,
//                             int nline, char *s)
//{
//    error("%s", s);
//    startpost("line was:");
//    postatom(nline, vec + message);
//    endpost();
//}
//
//void glist_readfrombinbuf(t_glist *x, t_binbuf *b, char *filename, int selectem)
//{
//    t_canvas *canvas = glist_getcanvas(x);
//    int cr = 0, natoms, nline, message, nextmsg = 0, i, j, nitems;
//    t_atom *vec;
//    t_gobj *gobj;
//
//    natoms = binbuf_getnatom(b);
//    vec = binbuf_getvec(b);
//
//
//    /* check for file type */
//    nline = canvas_scanbinbuf(natoms, vec, &message, &nextmsg);
//    if (nline != 1 && vec[message].a_type != A_SYMBOL &&
//        strcmp(vec[message].a_w.w_symbol->s_name, "data"))
//    {
//        pd_error(x, "%s: file apparently of wrong type", filename);
//        return;
//    }
//    /* read in templates and check for consistency */
//    while (1)
//    {
//        t_template *newtemplate, *existtemplate;
//        t_symbol *templatesym;
//        t_atom *templateargs = (t_atom*)getbytes(0);
//        int ntemplateargs = 0, newnargs;
//        nline = canvas_scanbinbuf(natoms, vec, &message, &nextmsg);
//        if (nline < 2)
//        {
//            t_freebytes(templateargs, sizeof (*templateargs) * ntemplateargs);
//            break;
//        }
//        else if (nline > 2)
//            canvas_readerror(natoms, vec, message, nline,
//                             "extra items ignored");
//        else if (vec[message].a_type != A_SYMBOL ||
//                 strcmp(vec[message].a_w.w_symbol->s_name, "template") ||
//                 vec[message + 1].a_type != A_SYMBOL)
//        {
//            canvas_readerror(natoms, vec, message, nline,
//                             "bad template header");
//            continue;
//        }
//        templatesym = canvas_makebindsym(vec[message + 1].a_w.w_symbol);
//        while (1)
//        {
//            nline = canvas_scanbinbuf(natoms, vec, &message, &nextmsg);
//            if (nline != 2 && nline != 3)
//                break;
//            newnargs = ntemplateargs + nline;
//            templateargs = (t_atom *)t_resizebytes(templateargs,
//                                                   sizeof(*templateargs) * ntemplateargs,
//                                                   sizeof(*templateargs) * newnargs);
//            templateargs[ntemplateargs] = vec[message];
//            templateargs[ntemplateargs + 1] = vec[message + 1];
//            if (nline == 3)
//                templateargs[ntemplateargs + 2] = vec[message + 2];
//            ntemplateargs = newnargs;
//        }
//        if (!(existtemplate = template_findbyname(templatesym)))
//        {
//            error("%s: template not found in current patch",
//                  templatesym->s_name);
//            t_freebytes(templateargs, sizeof (*templateargs) * ntemplateargs);
//            return;
//        }
//        newtemplate = template_new(templatesym, ntemplateargs, templateargs);
//        t_freebytes(templateargs, sizeof (*templateargs) * ntemplateargs);
//        if (!template_match(existtemplate, newtemplate))
//        {
//            error("%s: template doesn't match current one",
//                  templatesym->s_name);
//            pd_free(&newtemplate->t_pdobj);
//            return;
//        }
//        pd_free(&newtemplate->t_pdobj);
//    }
//    while (nextmsg < natoms)
//    {
//        canvas_readscalar(x, natoms, vec, &nextmsg, selectem);
//    }
//}

//*****

#define OBJ_NAME "exp.instance"

using namespace ceammc;

static void canvas_dopaste(t_canvas *x, t_binbuf *b)
{
    t_gobj *newgobj, *last, *g2;
    int dspstate = canvas_suspend_dsp(), nbox, count;
    t_symbol *asym = gensym("#A");
    /* save and clear bindings to symbols #a, $N, $X; restore when done */
    t_pd *boundx = s__X.s_thing, *bounda = asym->s_thing,
    *boundn = s__N.s_thing;
    asym->s_thing = 0;
    s__X.s_thing = &x->gl_pd;
    s__N.s_thing = 0;&pd_canvasmaker;
    
    canvas_editmode(x, 1.);
    glist_noselect(x);
    for (g2 = x->gl_list, nbox = 0; g2; g2 = g2->g_next) nbox++;
    
    //    paste_onset = nbox;
    //    paste_canvas = x;
    
    binbuf_eval(b, 0, 0, 0);
    for (g2 = x->gl_list, count = 0; g2; g2 = g2->g_next, count++)
        if (count >= nbox)
            glist_select(x, g2);
    //    paste_canvas = 0;
    canvas_resume_dsp(dspstate);
    canvas_dirty(x, 1);
    if (x->gl_mapped)
        sys_vgui("pdtk_canvas_getscroll .x%lx.c\n", x);
    //    if (!sys_noloadbang)
    //        glist_donewloadbangs(x);
    asym->s_thing = bounda;
    s__X.s_thing = boundx;
    s__N.s_thing = boundn;
}

static void exp_instance_delete(t_exp_instance* x)
{
    if (x->local_canvas)
    {
        if (x->local_canvas->gl_list)
        {
            //glist_noselect(x->local_canvas);
            //glist_select(x->local_canvas->gl_list, x->local_canvas->gl_list);
            glist_delete(x->local_canvas, x->local_canvas->gl_list);
        }
    }
}

static void exp_instance_update(t_exp_instance* x, t_symbol*s, int argc, t_atom* argv)
{
    
    if (x->global->ref())
    {
        
        // create instance
        
        if (!x->local_canvas)
        {
            //glist_init(x->local_canvas);
            
            printf("init\n");
            
            x->local_canvas = (t_canvas*)subcanvas_new(gensym(x->class_name.c_str())); //LISP lol
            x->local_canvas->gl_havewindow = 1;
            x->local_canvas->gl_env = 0;
            
            
        }
        else
        {
            //cleanup
            
           // t_gobj *y;
            
            if (x->local_canvas->gl_list)
            {
                //glist_noselect(x->local_canvas);
                //glist_select(x->local_canvas->gl_list, x->local_canvas->gl_list);
                glist_delete(x->local_canvas, x->local_canvas->gl_list);
            }
        }
        
        t_canvas *src_canvas = x->global->ref();
        
        t_binbuf *b1 = binbuf_new();
        
        canvas_saveto(src_canvas, b1);
        
        int blen=0;
        char *bchar;
        binbuf_gettext(b1, &bchar, &blen);
        
        
        int natoms = binbuf_getnatom(b1);
        t_atom* vec = binbuf_getvec(b1);
        postatom(natoms, vec);
        
        canvas_dopaste(x->local_canvas, b1);
        canvas_vis(x->local_canvas, 0);
        canvas_setcurrent(x->parent_canvas);
        
    }
}

static void exp_instance_setobject(t_exp_instance* x, t_symbol*id, int argc, t_atom* argv)
{
    if (argc<1)
    {
        error("no class name provided!");
        x->e_box.b_boxparameters.d_bordercolor = rgba_red;
        exp_instance_delete(x);
    }
    Atom a = argv[0];
    if (!a.isSymbol())
    {
        error("bad class name!");
        x->e_box.b_boxparameters.d_bordercolor = rgba_red;
    }
    
    x->class_name = a.asString();
    
    
    //oPDClass *pdClass =
    x->global = new oPDClass(a.asString(), OBJ_NAME);
    if (!x->global->ref())
    {
        error("class not found!");
        x->e_box.b_boxparameters.d_bordercolor = rgba_red;
    }
    else
    {
        
        exp_instance_update(x, 0, 0, 0);
        
    }
    
    
}

static void* exp_instance_new(t_symbol *id, int argc, t_atom *argv)
{
    
    
    
    
    t_exp_instance* x = reinterpret_cast<t_exp_instance*>(eobj_new(exp_instance_class));
    
   
    
    
    x->parent_canvas = canvas_getcurrent();
    
    ebox_new((t_ebox *)x, 0 );
    
    
    t_binbuf* d = binbuf_via_atoms(argc,argv);
    
    
    
    
    exp_instance_setobject(x, id, argc, argv);
    
    
    x->txt = etext_layout_create();
    x->fnt = efont_create(gensym("Monaco"), gensym(""), gensym("normal"), 12);
    
    canvas_setcurrent(x->parent_canvas);
    
    if (x && d)
    {
        
        ebox_attrprocess_viabinbuf(x, d);
        
        
    }
    
    ebox_ready((t_ebox *)x);
    
    
    x->e_box.b_boxparameters.d_boxfillcolor = rgba_greylight;
    char c1[] = "#00C0FF";
    x->e_box.b_boxparameters.d_bordercolor = hex_to_rgba(c1);
    
    return static_cast<void*>(x);
    
    
}

static void exp_instance_vis(t_exp_instance* x, t_symbol*, int argc, t_atom* argv)
{
    if (argc<1) return;
    Atom a = argv[0];
    
    if (!a.isFloat()) return;
    
    if (x->local_canvas)
    {
        post("vis");
        canvas_vis(x->local_canvas, (a.asInt()>0));
    }
}


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
        // EXAMPLE
        //            size = rect.width * 0.5;
        //            egraphics_set_color_hex(g, gensym("#00C0FF"));
        //            egraphics_circle(g, floor(size + 0.5), floor(size + 0.5), size * 0.9);
        //            egraphics_fill(g);
        //            ebox_end_layer((t_ebox*)x, bgl);
        
//        egraphics_rectangle(g, 0, 0, rect.width, rect.height);
//        egraphics_set_color_hex(g, gensym("#F0F0F0"));
//        egraphics_fill(g);
        
//        egraphics_rectangle(g, 0, 0, rect.width, rect.height);
//        egraphics_set_color_hex(g, gensym("#F0F0F0"));
//        egraphics_fill(g);
        
        printf("paint %f %f\n", rect.width, rect.height);
        
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
    
    eclass_addmethod(exp_instance_class,(t_typ_method)(exp_instance_setobject), ("object"), A_GIMME,0);
    
    
    eclass_addmethod(exp_instance_class, (method)(exp_instance_paint), ("paint"), A_NULL,0);
    eclass_addmethod(exp_instance_class, (method)(exp_instance_oksize), ("oksize"), A_GIMME,0);
    
//    eclass_addmethod(exp_instance_class, (t_typ_method)(exp_instance_inlet_proxy), ("anything"), A_GIMME, 0);
    
    eclass_register(CLASS_BOX, exp_instance_class);
    
}

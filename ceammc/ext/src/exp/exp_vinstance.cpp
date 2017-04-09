#include "oop_common.h"

#include <m_pd.h>

//
//t_eclass * exp_vinlet_class;
//typedef struct _exp_vinlet {
//    t_ebox e_box;
//
//    t_object* send_to;
//
//} t_exp_vinlet;
//
//static void* exp_vinlet_new(t_symbol* id, int argc, t_object* obj)
//{
//
//    t_exp_vinlet* x = reinterpret_cast<t_exp_vinlet*>(eobj_new(exp_vinlet_class));
//
//
//    ebox_new((t_ebox*)x, 0);
//
//    ebox_ready((t_ebox*)x);
//
//    if (argc>0)
//    {
//        x->send_to = obj;
//    }
//
//    x->e_box.b_boxparameters.d_boxfillcolor = rgba_greylight;
//    x->e_box.b_boxparameters.d_bordercolor = rgba_red;
//    x->e_box.b_boxparameters.d_borderthickness = 0;
//
//    x->e_box.b_boxparameters.d_hideiolets = 1;
//
//    return static_cast<void*>(x);
//}
//
//static void exp_vinstance_any(t_exp_vinstance* x, t_symbol* s, int argc, t_atom* argv);
//
//static void exp_vinlet_any(t_exp_vinlet* x, t_symbol* s, int argc, t_atom* argv)
//{
//    if (x->send_to)
//    {
//        exp_vinstance_any((t_exp_vinstance*)x->send_to, s, argc, argv);
//    }
//}
//
//void setup_custom_iolets()
//{
//
//    exp_vinlet_class = eclass_new(("exp.vinlet"),
//                                  reinterpret_cast<t_typ_method>(exp_vinlet_new),
//                                  reinterpret_cast<t_typ_method>(0),
//                                  sizeof(exp_vinlet_class), CLASS_PATCHABLE, A_GIMME, 0);
//
//    eclass_guiinit(exp_vinlet_class, 0);
//
//    CLASS_ATTR_DEFAULT(exp_vinlet_class, "size", 0, "10. 3.");
//
//    //hide standard CICM attributes
//    CLASS_ATTR_INVISIBLE(exp_vinlet_class, "fontname", 1);
//    CLASS_ATTR_INVISIBLE(exp_vinlet_class, "fontweight", 1);
//    CLASS_ATTR_INVISIBLE(exp_vinlet_class, "fontslant", 1);
//    CLASS_ATTR_INVISIBLE(exp_vinlet_class, "fontsize", 1);
//
//    // background / border color
//    CLASS_ATTR_LABEL(exp_vinlet_class, "bgcolor", 0, "Background Color");
//    CLASS_ATTR_ORDER(exp_vinlet_class, "bgcolor", 0, "1");
//    CLASS_ATTR_DEFAULT_SAVE_PAINT(exp_vinlet_class, "bgcolor", 0, "0.75 0.75 0.75 1.");
//    CLASS_ATTR_STYLE(exp_vinlet_class, "bgcolor", 0, "color");
//
//    eclass_addmethod(exp_vinlet_class, (t_typ_method)(exp_vinlet_any), ("anything"), A_GIMME, 0);
//
//}

//almost singleton lol

typedef struct _exp_vinstance t_exp_vinstance;

class t_viConnector {
private:
    t_exp_vinstance* from;
    int out_idx;

    t_canvas* canvas;

public:
    int start(t_exp_vinstance* from, int out_idx, t_canvas* canvas)
    {

        if (this->out_idx == out_idx) {
            //cancel

            this->from = 0;
            this->out_idx = -1;

            return 0;
        } else {
            this->from = from;
            this->out_idx = out_idx;

            this->canvas = canvas;

            return 1;
        }
    }

    void end(t_exp_vinstance* to, int in_idx, t_canvas* canvas)
    {
        if (this->from && (canvas == this->canvas) && (this->from != to)) {
            int out_obj = canvas_getindex(this->canvas, &((t_object*)this->from)->te_g);
            int in_obj = canvas_getindex(this->canvas, &((t_object*)to)->te_g);

            canvas_connect(this->canvas, out_obj, this->out_idx, in_obj, in_idx);

            //post("connect %i %i", out_idx,in_idx);
        }
    }
};

//lol
static t_viConnector viConnector;

#pragma mark -

t_eclass* exp_vinstance_class;

typedef struct _exp_vinstance {
    t_ebox e_box;

    t_canvas* parent_canvas; //patch

    //t_canvas *local_canvas;         //instance

    OPClasses* op_class; //class
    OPInstance* instance;

    t_etext* txt;
    t_efont* fnt;
    t_efont* fnt2;

    t_etext** methods_txt;
    int methods_txt_count;
    t_etext** property_txt;
    int property_txt_count;

    t_sample* buffer;
    int vec_size;

    t_inlet* in1;
    t_outlet* out1;

    t_inlet** inlets;
    t_outlet** outlets;

    int inlet_count;
    int outlet_count;

    int sel_outlet;

} t_exp_vinstance;

#define OBJ_NAME "exp.vinstance"

using namespace ceammc;

static void exp_vinstance_delete(t_exp_vinstance* x)
{
    if (x->instance) {
        x->instance->freeInstanceOut(x->out1);
        x->instance->release();
        x->instance = 0;
    }
}

static void exp_vinstance_redraw(t_exp_vinstance* x)
{
    ebox_invalidate_layer((t_ebox*)x, gensym("background_layer"));
    ebox_invalidate_layer((t_ebox*)x, gensym("iolets"));
    ebox_redraw((t_ebox*)x);
}
#pragma mark -
//CEAMMC EXPERIMENTAL
extern "C" {
EXTERN void inlet_set_yoffset(t_inlet* inlet, float y);
EXTERN void outlet_set_yoffset(t_outlet* outlet, float y);
}

#pragma mark -

static void exp_vinstance_freeinstance(t_exp_vinstance* x, t_symbol* id, int argc, t_atom* argv);

static void exp_vinstance_setclass(t_exp_vinstance* x, t_symbol* id, int argc, t_atom* argv)
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

    if (!x->op_class->ref()) {
        error("class not found!");
        return;

    } else {

        if (x->instance)
            exp_vinstance_freeinstance(x, 0, 0, 0);

        //x->op_class->ref()->class_name = a.asString();

        //char c1[] = "#00C0FF";
        //x->e_box.b_boxparameters.d_bordercolor = hex_to_rgba(c1);

        canvas_setcurrent(x->parent_canvas);

        //x->instance = new OPInstance(x->op_class->ref());
        //if (x->instance)
        //x->instance->addInstanceOut(x->out1);

        //move
        float w = 90;
        float h = 90; //dummy

        AtomList argv;
        argv.append(Atom(w));
        argv.append(Atom(h));
        eobj_attr_setvalueof(x, gensym("size"), 2, argv.toPdData());

        //MOVE

        int m_c = 0;
        if (x->op_class)
            if (x->op_class->ref()) {
                AtomList methodNames = x->op_class->ref()->getMethodList();
                m_c = (int)methodNames.size();

                AtomList propertyNames = x->op_class->ref()->getPropertyList();
                m_c += (int)propertyNames.size();
            }

        //IO

        if (x->inlets) {
            for (int i = 0; i < x->inlet_count; i++) {
                if (x->inlets[i]) {
                    inlet_free(x->inlets[i]);
                }
            }
            delete x->inlets;
        }

        if (x->outlets) {
            for (int i = 0; i < x->outlet_count; i++) {
                if (x->outlets[i]) {
                    outlet_free(x->outlets[i]);
                }
            }
            delete x->outlets;
        }

        if (m_c > 0) {
            x->inlets = new t_inlet*[m_c];
            x->outlets = new t_outlet*[m_c];

            for (int i = 0; i < m_c; i++) {

                x->inlets[i] = inlet_new(&x->e_box.b_obj.o_obj, &x->e_box.b_obj.o_obj.ob_pd, 0, 0);
                x->outlets[i] = outlet_new((t_object*)x, &s_anything);

                inlet_set_yoffset(x->inlets[i], 7.5 + 15 * (i + 1));
                outlet_set_yoffset(x->outlets[i], -7.5 - (m_c * 15) + 15 * (i + 1));
            }

            outlet_set_yoffset(x->out1, -7.5 - (m_c * 15));
        }

        exp_vinstance_redraw(x);
    }
}

static void exp_vinstance_newinstance(t_exp_vinstance* x, t_symbol* id, int argc, t_atom* argv)
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

    if (x->op_class)
        if (x->op_class->ref()->class_name != a.asString()) {
            error("class %s does not match the box class %s", a.asString().c_str(), x->op_class->ref()->class_name.c_str());
            return;
        }

    x->op_class = new OPClasses(a.asString(), OBJ_NAME);

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
        //if (x->instance)
        x->instance->addInstanceOut(x->out1);

        //move
        float w = 90;
        float h = 90; //dummy

        AtomList argv;
        argv.append(Atom(w));
        argv.append(Atom(h));
        eobj_attr_setvalueof(x, gensym("size"), 2, argv.toPdData());

        exp_vinstance_redraw(x);
    }
}

static void exp_vinstance_freeinstance(t_exp_vinstance* x, t_symbol* id, int argc, t_atom* argv)
{

    exp_vinstance_delete(x);

    x->e_box.b_boxparameters.d_bordercolor = rgba_red;

    //move
    float w = 90;
    float h = 14; //dummy

    AtomList argv_;
    argv_.append(Atom(w));
    argv_.append(Atom(h));
    eobj_attr_setvalueof(x, gensym("size"), 2, argv_.toPdData());

    exp_vinstance_redraw(x);
}

static void exp_vinstance_update(t_exp_vinstance* x, t_symbol* s, int argc, t_atom* argv)
{
    std::string className = x->instance->class_name;

    exp_vinstance_freeinstance(x, 0, 0, 0);
    exp_vinstance_newinstance(x, 0, 1, AtomList(Atom(gensym(className.c_str()))).toPdData());
}
#pragma mark -

static void exp_vinstance_setobject(t_exp_vinstance* x, t_symbol* s, int argc, t_atom* argv)
{
    if (argc < 1) {

        exp_vinstance_freeinstance(x, 0, 0, 0);

    } else {
        if (x->instance)
            if (x->instance->canvas) {

                canvas_vis(x->instance->canvas, 0);

                exp_vinstance_freeinstance(x, 0, 0, 0);
            }

        Atom a = argv[0];
        //postatom(argc, argv); post("");
        x->instance = OPInstance::findBySymbol(a.asSymbol());

        if (x->instance) {
            x->instance->retain();
            x->instance->addInstanceOut(x->out1);

            char c1[] = "#00C0FF";
            x->e_box.b_boxparameters.d_bordercolor = hex_to_rgba(c1);

            exp_vinstance_redraw(x);

            if (!x->instance->canvas) {
                x->e_box.b_boxparameters.d_bordercolor = rgba_red;

                exp_vinstance_redraw(x);
            }

            x->buffer = x->instance->getBufferFor(gensym("out1"), x->vec_size);

            if (!x->buffer) {
                error("buffer error!");
                x->buffer = new t_sample[x->vec_size];
            }
        } else {
            error("instance not found!");

            x->e_box.b_boxparameters.d_bordercolor = rgba_red;

            exp_vinstance_redraw(x);
        }
    }
}

static void exp_vinstance_getobject(t_exp_vinstance* x, t_symbol* s, int argc, t_atom* argv)
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
static void exp_vinstance_getproperty(t_exp_vinstance* x, t_symbol* id, int argc, t_atom* argv)
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

static void exp_vinstance_setproperty(t_exp_vinstance* x, t_symbol* id, int argc, t_atom* argv)
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

//t_int *exp_vinstance_perform(t_int *w)
//{
//    //t_exp_vinstance *x = (t_exp_vinstance *)(w[1]);
//    t_sample  *in1 =    (t_sample *)(w[2]);
//    t_sample  *out =    (t_sample *)(w[3]);
//    int          n =           (int)(w[4]);
//
//    while (n--) *out++ = (*in1++);
//
//    return (w+6);
//}
//static void exp_vinstance_dsp(t_exp_vinstance* x, t_signal **sp)
//{
//
//    dsp_add(exp_vinstance_perform, 4, x,
//            sp[0]->s_vec, sp[1]->s_vec, sp[0]->s_n);
//}

static void exp_vinstance_perform(t_exp_vinstance* x, t_object*,
    t_sample** ins, long,
    t_sample**, long,
    long sampleframes, long, void*)
{

    //    t_sample* in = ins[0];
    //    t_sample* out;

    std::copy(x->buffer, x->buffer + sampleframes, ins[0]);
}

static void exp_vinstance_dsp(t_exp_vinstance* x, t_object* dsp, short* /*count*/, double /*samplerate*/, long vec_size /*maxvectorsize*/, long /*flags*/)
{
    x->vec_size = (int)vec_size;

    object_method(dsp, gensym("dsp_add"), x, reinterpret_cast<method>(exp_vinstance_perform), 0, NULL);
}

#pragma mark -

static void exp_vinstance_any(t_exp_vinstance* x, t_symbol* s, int argc, t_atom* argv)
{
    if (x->instance) {
        AtomList list = Atom(s);
        list.append(AtomList(argc, argv));

        x->instance->callMethod(list);
    }
}

static void exp_vinstance_methodlist(t_exp_vinstance* x, t_symbol*, int argc, t_atom* argv)
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

static void exp_vinstance_refcount(t_exp_vinstance* x, t_symbol*, int argc, t_atom* argv)
{
    if (x->instance) {
        post("reference count: %i", x->instance->getRefCount());
    }
}

#pragma mark -
#pragma mark object box

static void* exp_vinstance_new(t_symbol* id, int argc, t_atom* argv)
{

    t_exp_vinstance* x = reinterpret_cast<t_exp_vinstance*>(eobj_new(exp_vinstance_class));

    x->parent_canvas = canvas_getcurrent();

    x->op_class = new OPClasses("", OBJ_NAME);

    ebox_new((t_ebox*)x, 0);
    t_binbuf* d = binbuf_via_atoms(argc, argv);

    x->txt = etext_layout_create();
    x->fnt = efont_create(gensym("Monaco"), gensym("normal"), gensym(""), 10);
    x->fnt2 = efont_create(gensym("Monaco"), gensym("normal"), gensym(""), 10);

    canvas_setcurrent(x->parent_canvas);

    if (x && d) {
        ebox_attrprocess_viabinbuf(x, d);
    }

    ebox_ready((t_ebox*)x);

    x->e_box.b_boxparameters.d_boxfillcolor = rgba_greylight;
    x->e_box.b_boxparameters.d_bordercolor = rgba_red;

    x->out1 = outlet_new((t_object*)x, &s_anything);

    x->in1 = inlet_new(&x->e_box.b_obj.o_obj, &x->e_box.b_obj.o_obj.ob_pd, 0, 0);

    //if (x->e_box.b_obj.o_obj.te_inlet)

    inlet_set_yoffset(x->in1, 7.5);
    outlet_set_yoffset(x->out1, -7.5);

    //x->in2 = inlet_new(&x->e_box.b_obj.o_obj, &x->e_box.b_obj.o_obj.ob_pd, &s_signal, &s_signal);

    x->vec_size = 64; //test
    x->instance = 0;

    x->e_box.b_boxparameters.d_hideiolets = 1;

    x->methods_txt_count = 0;
    x->property_txt_count = 0;

    x->sel_outlet = -1;

    return static_cast<void*>(x);
}

static void exp_vinstance_free(t_exp_vinstance* x)
{
    exp_vinstance_delete(x);
}

static void exp_vinstance_vis(t_exp_vinstance* x, t_symbol*, int argc, t_atom* argv)
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
#pragma mark -

static void exp_vinstance_iolets(t_object* z, t_object* view)
{
    // main inlet

    t_symbol* bgl = gensym("iolets");
    //float size;
    t_rect rect;
    ebox_get_rect_for_view((t_ebox*)z, &rect);

    t_exp_vinstance* zx = (t_exp_vinstance*)z;

    t_elayer* g = ebox_start_layer((t_ebox*)z, bgl, rect.width, rect.height);

    if (g) {
        //        egraphics_circle(g, 0, 7.5, 3);
        //        egraphics_set_color_hex(g, gensym("#303030"));
        //        egraphics_fill(g);

        egraphics_rectangle(g, -1, 5.5, 5, 2);
        egraphics_set_color_hex(g, gensym("#303030"));
        egraphics_stroke(g);

        egraphics_rectangle(g, rect.width - 5, 5.5, 5, 2);
        egraphics_set_color_hex(g, gensym("#303030"));
        egraphics_stroke(g);

        //MOVE

        //        if (zx->op_class)
        //            if (zx->op_class->ref())
        //            {
        //                AtomList methodNames = zx->op_class->ref()->getMethodList();
        //                m_c = (int)methodNames.size();
        //
        //                AtomList propertyNames = zx->instance->getPropertyList();
        //                p_c = (int)propertyNames.size();
        //            }

        //if (zx->instance)
        if (zx->op_class)
            if (zx->op_class->ref()) {
                AtomList methodNames = zx->op_class->ref()->getMethodList();
                int m_c = (int)methodNames.size();
                //post("method count %i", m_c);

                if (zx->methods_txt) {
                    for (int i = 0; i < zx->methods_txt_count; i++) {
                        etext_layout_destroy(zx->methods_txt[i]);
                    }
                    delete zx->methods_txt;
                }

                zx->methods_txt = new t_etext*[m_c];

                for (int i = 0; i < m_c; i++) {
                    float y = 22.5 + 15 * i;

                    egraphics_rectangle(g, -1, y - 2, 5, 2);
                    egraphics_set_color_hex(g, gensym("#707070"));
                    egraphics_stroke(g);

                    egraphics_rectangle(g, rect.width - 5, y - 2, 5, 2);
                    egraphics_set_color_hex(g, gensym("#707070"));
                    egraphics_stroke(g);

                    //move!
                    zx->methods_txt[i] = etext_layout_create();

                    etext_layout_set(zx->methods_txt[i], methodNames.at(i).asString().c_str(), zx->fnt, 12, y + 5.5, rect.width - 10, rect.height / 2, ETEXT_DOWN_LEFT, ETEXT_JLEFT, ETEXT_NOWRAP);
                    etext_layout_draw(zx->methods_txt[i], g);
                }

                float method_y = 22.5 + 15 * m_c;

                //

                AtomList propertyNames = zx->op_class->ref()->getPropertyList();
                int p_c = (int)propertyNames.size();
                //post("property count %i", p_c);

                if (zx->property_txt) {
                    for (int i = 0; i < zx->property_txt_count; i++) {
                        //zx->methods_txt[i]
                        etext_layout_destroy(zx->property_txt[i]);
                    }
                    delete zx->property_txt;
                }

                zx->property_txt = new t_etext*[p_c];

                for (int i = 0; i < p_c; i++) {
                    float y = method_y + 15 * i;

                    egraphics_circle(g, 0, y, 3.5);
                    egraphics_set_color_hex(g, gensym("#707070"));
                    egraphics_fill(g);
                    egraphics_circle(g, 0, y, 3.5);
                    egraphics_set_color_hex(g, gensym("#303030"));
                    egraphics_stroke(g);

                    egraphics_circle(g, rect.width, y, 3.5);
                    egraphics_set_color_hex(g, gensym("#707070"));
                    egraphics_fill(g);
                    egraphics_circle(g, rect.width, y, 3.5);
                    egraphics_set_color_hex(g, gensym("#303030"));
                    egraphics_stroke(g);

                    //move!
                    zx->property_txt[i] = etext_layout_create();

                    etext_layout_set(zx->property_txt[i], propertyNames.at(i).asString().c_str(), zx->fnt, 12, y + 5.5, rect.width - 10, rect.height / 2, ETEXT_DOWN_LEFT, ETEXT_JLEFT, ETEXT_NOWRAP);
                    etext_layout_draw(zx->property_txt[i], g);
                }

                egraphics_line(g, 0, method_y - 7.5, rect.width, method_y - 7.5);
                egraphics_set_color_rgba(g, &zx->e_box.b_boxparameters.d_bordercolor);
                egraphics_stroke(g);
            }

        if (zx->sel_outlet >= 0) {
            float y = 6.5 + 15 * zx->sel_outlet;

            egraphics_circle(g, rect.width - 5, y, 5);
            egraphics_set_color_hex(g, gensym("#FFFFFF"));
            egraphics_stroke(g);
        }
    }

    ebox_end_layer((t_ebox*)z, bgl);
    ebox_paint_layer((t_ebox*)z, bgl, 0., 0.);
}

static void exp_vinstance_paint(t_object* z, t_object* view)
{
    t_symbol* bgl = gensym("background_layer");
    //float size;
    t_rect rect;
    ebox_get_rect_for_view((t_ebox*)z, &rect);

    t_exp_vinstance* zx = (t_exp_vinstance*)z;

    t_elayer* g = ebox_start_layer((t_ebox*)z, bgl, rect.width, rect.height);

    if (g) {

        {

            std::string disp_name;
            if (zx->instance)
                disp_name = zx->instance->class_name;
            else
                disp_name = "<empty>";

            if (zx->op_class && !zx->instance)
                if (zx->op_class->ref())
                    disp_name = "<" + zx->op_class->ref()->class_name + ">";

            //= (zx->instance) ? zx->instance->class_name : "<empty>";

            etext_layout_set(zx->txt, disp_name.c_str(), zx->fnt, 12, 14, rect.width - 10, rect.height / 2, ETEXT_DOWN_LEFT, ETEXT_JLEFT, ETEXT_NOWRAP);
            etext_layout_draw(zx->txt, g);
        }

        egraphics_line(g, 0, 14, rect.width, 14);
        egraphics_set_color_rgba(g, &zx->e_box.b_boxparameters.d_bordercolor);
        egraphics_stroke(g);
    }

    ebox_end_layer((t_ebox*)z, bgl);
    ebox_paint_layer((t_ebox*)z, bgl, 0., 0.);

    exp_vinstance_iolets(z, view);
}

static const char* my_cursorlist[] = {
    "left_ptr",
    "center_ptr",
    "sb_v_double_arrow",
    "plus",
    "hand2",
    "circle",
    "X_cursor",
    "bottom_side",
    "bottom_right_corner",
    "right_side",
    "double_arrow",
    "exchange",
    "xterm"
};

void ebox_set_cursor_t(t_ebox* x, int cursor)
{
    cursor = (int)pd_clip_minmax(cursor, 0, 12);
    sys_vgui("%s configure -cursor %s\n", x->b_drawing_id->s_name, my_cursorlist[cursor]);
}

extern "C" {

static long modifier_wrapper(long mod)
{
#ifdef __APPLE__
    if (mod >= 256) {
        mod -= 256;
    }
#elif _WINDOWS

    if (mod >= 131072) {
        mod -= 131072;
        mod += EMOD_ALT;
    }
/*else
     mod -= 8;*/
#else
    if (mod == 24) //right click
        mod = EMOD_CMD;
    else if (mod & EMOD_CMD) {
        mod ^= EMOD_CMD;
        mod |= EMOD_ALT;
    }
#endif
    //post("MOD : %ld", mod);
    return mod;
}

static char is_for_box(t_ebox* x, long mod)
{
    return (!x->b_obj.o_canvas->gl_edit || (x->b_obj.o_canvas->gl_edit && mod == EMOD_CMD));
}

static void exp_vinstance_mousemove(t_exp_vinstance* xx, t_symbol* s, int argc, t_atom* argv)
{
    t_ebox* x = &xx->e_box;
    int right, bottom;
    t_pt mouse;
    t_atom av[2];
    long modif = modifier_wrapper((long)atom_getfloat(argv + 2));
    t_eclass* c = (t_eclass*)exp_vinstance_class; //eobj_getclass(x);
    if (!x->b_mouse_down) {
        if (is_for_box(x, modif)) {
            if (!(x->b_flags & EBOX_IGNORELOCKCLICK)) {
                ebox_set_cursor(x, 1);
                if (c->c_widget.w_mousemove) {
                    mouse.x = atom_getfloat(argv);
                    mouse.y = atom_getfloat(argv + 1);
                    c->c_widget.w_mousemove(x, x->b_obj.o_canvas, mouse, modif);
                }
            } else {
                ebox_set_cursor(x, 0);
            }
        } else if (!x->b_isinsubcanvas) {
            mouse.x = atom_getfloat(argv);
            mouse.y = atom_getfloat(argv + 1);
            x->b_selected_outlet = -1;
            x->b_selected_inlet = -1;
            x->b_selected_item = EITEM_NONE;
            sys_vgui("eobj_canvas_motion %s 0\n", x->b_canvas_id->s_name);

            right = (int)(x->b_rect.width + x->b_boxparameters.d_borderthickness * 2.);
            bottom = (int)(x->b_rect.height + x->b_boxparameters.d_borderthickness * 2.);

            //SIDE I/O

            mouse.x = atom_getfloat(argv);
            mouse.y = atom_getfloat(argv + 1);

            //post("mouse %f %f", mouse.x, mouse.y);

            if ((mouse.x < 4) && ((int(mouse.y) % 15) > 7) && ((int(mouse.y) % 15) < 13)) {
                xx->e_box.b_selected_inlet = floor(mouse.y / 15);
                ebox_set_cursor_t(&xx->e_box, 4);
                //post("m in");
                return;
            } else {
                xx->e_box.b_selected_inlet = -1;
                ebox_set_cursor_t(&xx->e_box, 0);
            }

            if ((mouse.x > 86) && ((int(mouse.y) % 15) > 7) && ((int(mouse.y) % 15) < 13)) {
                xx->e_box.b_selected_outlet = floor(mouse.y / 15);
                ebox_set_cursor_t(&xx->e_box, 4);
                //post("m out");
                return;
            } else {
                xx->e_box.b_selected_outlet = -1;
                ebox_set_cursor_t(&xx->e_box, 0);
            }

            // BOX //
            ebox_set_cursor(x, 4);
            ebox_invalidate_layer(x, s_eboxio);
            ebox_redraw(x);
        } else {
            sys_vgui("eobj_canvas_motion %s 0\n", x->b_canvas_id->s_name);
        }
    } else {
        if (is_for_box(x, modif)) {
            if (c->c_widget.w_mousedrag && !(x->b_flags & EBOX_IGNORELOCKCLICK)) {
                mouse.x = atom_getfloat(argv);
                mouse.y = atom_getfloat(argv + 1);
                c->c_widget.w_mousedrag(x, x->b_obj.o_canvas, mouse, modif);
            }
        } else if (!x->b_isinsubcanvas) {
            mouse.x = atom_getfloat(argv);
            mouse.y = atom_getfloat(argv + 1);

            //SIDE I/O

            mouse.x = atom_getfloat(argv);
            mouse.y = atom_getfloat(argv + 1);

            //post("mouse %f %f", mouse.x, mouse.y);

            if ((mouse.x < 4) && ((int(mouse.y) % 15) > 7) && ((int(mouse.y) % 15) < 13)) {
                xx->e_box.b_selected_inlet = floor(mouse.y / 15);
                ebox_set_cursor_t(&xx->e_box, 4);
                //post("m in");
                return;
            } else {
                xx->e_box.b_selected_inlet = -1;
                ebox_set_cursor_t(&xx->e_box, 0);
            }

            if ((mouse.x > 86) && ((int(mouse.y) % 15) > 7) && ((int(mouse.y) % 15) < 13)) {
                xx->e_box.b_selected_outlet = floor(mouse.y / 15);
                ebox_set_cursor_t(&xx->e_box, 4);
                //post("m out");
                return;
            } else {
                xx->e_box.b_selected_outlet = -1;
                ebox_set_cursor_t(&xx->e_box, 0);
            }

            if (x->b_selected_item == EITEM_NONE) {
                sys_vgui("eobj_canvas_motion %s 0\n", x->b_canvas_id->s_name);
            } else if (!(x->b_flags & EBOX_GROWNO)) {
                if (x->b_flags & EBOX_GROWLINK) {
                    if (x->b_selected_item == EITEM_BOTTOM) {
                        atom_setfloat(av, x->b_rect_last.width + (mouse.y - x->b_rect_last.height));
                        atom_setfloat(av + 1, mouse.y);
                    } else if (x->b_selected_item == EITEM_RIGHT) {
                        atom_setfloat(av, mouse.x);
                        atom_setfloat(av + 1, x->b_rect_last.height + (mouse.x - x->b_rect_last.width));
                    } else if (x->b_selected_item == EITEM_CORNER) {
                        if (mouse.y > mouse.x) {
                            atom_setfloat(av, mouse.y);
                            atom_setfloat(av + 1, mouse.y);
                        } else {
                            atom_setfloat(av, mouse.x);
                            atom_setfloat(av + 1, mouse.x);
                        }
                    }
                } else if (x->b_flags & EBOX_GROWINDI) {
                    if (x->b_selected_item == EITEM_BOTTOM) {
                        atom_setfloat(av, x->b_rect_last.width);
                        atom_setfloat(av + 1, mouse.y);
                    } else if (x->b_selected_item == EITEM_RIGHT) {
                        atom_setfloat(av, mouse.x);
                        atom_setfloat(av + 1, x->b_rect_last.height);
                    } else if (x->b_selected_item == EITEM_CORNER) {
                        atom_setfloat(av, mouse.x);
                        atom_setfloat(av + 1, mouse.y);
                    }
                }
                mess3((t_pd*)x, s_attr_size, s_attr_size, (void*)2, (void*)av);
            }
        } else {
            sys_vgui("eobj_canvas_motion %s 1\n", x->b_canvas_id->s_name);
        }
    }
}
}

#pragma mark -

static void exp_vinstance_oksize(t_object* z, t_rect* newrect)
{
    int m_c = 0;
    int p_c = 0;

    t_exp_vinstance* zx = (t_exp_vinstance*)z;

    if (zx->instance) {
        AtomList methodNames = zx->instance->getMethodList();
        m_c = (int)methodNames.size();

        AtomList propertyNames = zx->instance->getPropertyList();
        p_c = (int)propertyNames.size();
    }

    if (zx->op_class)
        if (zx->op_class->ref()) {
            AtomList methodNames = zx->op_class->ref()->getMethodList();
            m_c = (int)methodNames.size();

            AtomList propertyNames = zx->op_class->ref()->getPropertyList();
            p_c = (int)propertyNames.size();
        }

    newrect->width = 90;
    newrect->height = 14. + 15 * m_c + 15 * p_c;
}

static void exp_vinstance_click(t_exp_vinstance* x, t_object* view, t_pt mouse, long modifiers) //(t_exp_vinstance* x, t_symbol* s, int argc, t_atom* argv)
{
    if (x->instance && (x->e_box.b_selected_inlet == -1) && (x->e_box.b_selected_outlet = -1))
        if (x->instance->canvas)
            canvas_vis(x->instance->canvas, 1);

    // I/O
    //t_pt mouse;
    //mouse.x = atom_getfloat(argv);
    //mouse.y = atom_getfloat(argv+1);

    //post("mouse %f %f", mouse.x, mouse.y);

    if ((mouse.x < 10) && ((int(mouse.y) % 15) > 2) && ((int(mouse.y) % 15) < 13)) {
        x->e_box.b_selected_inlet = floor(mouse.y / 15);
        ebox_set_cursor_t(&x->e_box, 4);
        //post("m in");

    } else {
        x->e_box.b_selected_inlet = -1;
        ebox_set_cursor_t(&x->e_box, 0);
    }

    if ((mouse.x > 80) && ((int(mouse.y) % 15) > 2) && ((int(mouse.y) % 15) < 13)) {
        x->e_box.b_selected_outlet = floor(mouse.y / 15);
        ebox_set_cursor_t(&x->e_box, 4);
        //post("m out");

    } else {
        x->e_box.b_selected_outlet = -1;
        ebox_set_cursor_t(&x->e_box, 0);
    }

    //

    if (x->e_box.b_selected_outlet >= 0) {
        //post("start connect");

        int res = viConnector.start(x, x->e_box.b_selected_outlet, x->parent_canvas);

        x->sel_outlet = (res) ? x->e_box.b_selected_outlet : -1;

        exp_vinstance_redraw(x);
    }
    if (x->e_box.b_selected_inlet >= 0) {
        //post("trying to connect to %i", x->e_box.b_selected_inlet);

        viConnector.end(x, x->e_box.b_selected_inlet, x->parent_canvas);
    }
}

#pragma mark -
extern "C" void setup_exp0x2evinstance()
{

    exp_vinstance_class = eclass_new((OBJ_NAME),
        reinterpret_cast<t_typ_method>(exp_vinstance_new),
        reinterpret_cast<t_typ_method>(0),
        sizeof(t_exp_vinstance), CLASS_NOINLET, A_GIMME, 0);

    //eclass_dspinit(exp_vinstance_class);
    eclass_guiinit(exp_vinstance_class, 0);

    CLASS_ATTR_DEFAULT(exp_vinstance_class, "size", 0, "90. 15.");

    //hide standard CICM attributes
    CLASS_ATTR_INVISIBLE(exp_vinstance_class, "fontname", 1);
    CLASS_ATTR_INVISIBLE(exp_vinstance_class, "fontweight", 1);
    CLASS_ATTR_INVISIBLE(exp_vinstance_class, "fontslant", 1);
    CLASS_ATTR_INVISIBLE(exp_vinstance_class, "fontsize", 1);

    // background / border color
    CLASS_ATTR_LABEL(exp_vinstance_class, "bgcolor", 0, "Background Color");
    CLASS_ATTR_ORDER(exp_vinstance_class, "bgcolor", 0, "1");
    CLASS_ATTR_DEFAULT_SAVE_PAINT(exp_vinstance_class, "bgcolor", 0, "0.75 0.75 0.75 1.");
    CLASS_ATTR_STYLE(exp_vinstance_class, "bgcolor", 0, "color");

    eclass_addmethod(exp_vinstance_class, (t_typ_method)(exp_vinstance_update), ("update"), A_NULL, 0);

    eclass_addmethod(exp_vinstance_class, (t_typ_method)(exp_vinstance_newinstance), ("new"), A_GIMME, 0);
    eclass_addmethod(exp_vinstance_class, (t_typ_method)(exp_vinstance_freeinstance), ("free"), A_GIMME, 0);

    eclass_addmethod(exp_vinstance_class, (t_typ_method)exp_vinstance_methodlist, ("methodlist"), A_NULL, 0);
    eclass_addmethod(exp_vinstance_class, (t_typ_method)exp_vinstance_refcount, ("refcount"), A_NULL, 0);

    eclass_addmethod(exp_vinstance_class, (t_typ_method)(exp_vinstance_getobject), ("getobject"), A_GIMME, 0);
    eclass_addmethod(exp_vinstance_class, (t_typ_method)(exp_vinstance_setobject), ("setobject"), A_GIMME, 0);

    //todo move to 'anything', use @prop / @prop?
    eclass_addmethod(exp_vinstance_class, (t_typ_method)(exp_vinstance_getproperty), ("getp"), A_GIMME, 0);
    eclass_addmethod(exp_vinstance_class, (t_typ_method)(exp_vinstance_setproperty), ("setp"), A_GIMME, 0);

    eclass_addmethod(exp_vinstance_class, (t_typ_method)(exp_vinstance_any), ("anything"), A_GIMME, 0);

    eclass_addmethod(exp_vinstance_class, (method)(exp_vinstance_paint), ("paint"), A_NULL, 0);
    eclass_addmethod(exp_vinstance_class, (method)(exp_vinstance_oksize), ("oksize"), A_GIMME, 0);

    eclass_addmethod(exp_vinstance_class, (t_typ_method)exp_vinstance_click, ("mousedown"), A_GIMME, 0);
    eclass_addmethod(exp_vinstance_class, (t_typ_method)exp_vinstance_click, ("mousedrag"), A_GIMME, 0);

    //audio
    eclass_addmethod(exp_vinstance_class, (t_typ_method)exp_vinstance_dsp, ("dsp"), A_NULL, 0);

    eclass_register(CLASS_BOX, exp_vinstance_class);

    eclass_addmethod(exp_vinstance_class, (t_typ_method)(exp_vinstance_setclass), ("class"), A_GIMME, 0);

    //eclass_addmethod(exp_vinstance_class, (t_method)exp_vinstance_mousemove, ("mousemove"), A_NULL, 0);

    class_addmethod(&exp_vinstance_class->c_class, (t_method)exp_vinstance_mousemove, gensym("mousemove"), A_GIMME, 0);

    //exp_vinstance_class->c_widget.w_visfn = ebox_vis_ext;

    //CLASS_MAINSIGNALIN((t_class*)exp_vinstance_class, t_exp_vinstance, in_f);

    //setup_custom_iolets();

    //TODO
}

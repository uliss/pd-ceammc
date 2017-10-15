#include "oop_common.h"

#define OBJ_NAME "exp.signal~"

t_eclass* exp_signal_class;

using namespace ceammc;

struct t_exp_signal {
    t_ebox e_box;

    t_canvas* parent_canvas;

    t_symbol* sig_name;

    OPInstance* instance;

    t_sample* buffer;

    t_inlet* in1;
};

static void* exp_signal_new(t_symbol* id, int argc, t_atom* argv)
{

    if (argc < 1) {
        error("missing method name argument");
        return 0;
    }

    t_exp_signal* x = reinterpret_cast<t_exp_signal*>(eobj_new(exp_signal_class));
    x->parent_canvas = canvas_getcurrent();

    t_binbuf* d = binbuf_via_atoms(argc, argv);

    if (x && d) {
        ebox_attrprocess_viabinbuf(x, d);
    }

    Atom a = (argv[0]);
    x->sig_name = a.asSymbol();

    x->instance = OPInstance::findByCanvas(x->parent_canvas);
    
    OPClass *op_class = OPClass::findByCanvas(x->parent_canvas);
    
    if (op_class)
    {
        op_class->addSignal(a.asString(), "");
    }
    
    x->in1 = inlet_new(&x->e_box.b_obj.o_obj, &x->e_box.b_obj.o_obj.ob_pd, &s_signal, &s_signal);

    ebox_ready((t_ebox*)x);

    x->e_box.b_boxparameters.d_boxfillcolor = rgba_greylight;
    x->e_box.b_boxparameters.d_bordercolor = rgba_green;

    //x->out1 = outlet_new((t_object*)x, &s_anything);

    return static_cast<void*>(x);
}

static void exp_signal_free(t_exp_signal* x, t_symbol* id, int argc, t_atom* argv)
{
    if (x->instance)
        x->instance->freeSignal(x->sig_name);
    //temp
    x->buffer = 0;
}

#pragma mark -

static void exp_signal_perform(t_exp_signal* x, t_object*,
    t_sample** ins, long,
    t_sample**, long,
    long sampleframes, long, void*)
{

    t_sample* in = ins[0];
    
    if (x->buffer)
        std::copy(in, in + sampleframes, x->buffer);
}

static void exp_signal_dsp(t_exp_signal* x, t_object* dsp, short* /*count*/, double /*samplerate*/, long vec_size /*maxvectorsize*/, long /*flags*/)
{
    
    if (x->instance)
    {
        post("signal~ dsp new buffer: %i",(int)vec_size);
        
        x->buffer = x->instance->getBufferFor(x->sig_name, (int)vec_size);
    }

    if (!x->buffer) {
        error("signal~ buffer error!");
        x->buffer = new t_sample[vec_size];
    }
    object_method(dsp, gensym("dsp_add"), x, reinterpret_cast<method>(exp_signal_perform), 0, NULL);
}

//#pragma mark -

//static void *exp_signal_out_new(t_symbol *id, int argc, t_atom *argv)
//{
//
//    if (argc<1)
//    {
//        error("missing signal name argument");
//        return 0;
//    }
//
//    t_exp_signal* x = reinterpret_cast<t_exp_signal*>(eobj_new(exp_signal_class));
//    x->parent_canvas = canvas_getcurrent();
//
//    t_binbuf* d = binbuf_via_atoms(argc,argv);
//
//    if (x && d)
//    {
//        ebox_attrprocess_viabinbuf(x, d);
//    }
//
//    Atom a = (argv[0]);
//    x->sig_name = a.asSymbol();
//
//    x->instance = OPInstance::findByCanvas(x->parent_canvas);
//
//    x->in1=inlet_new(&x->e_box.b_obj.o_obj, &x->e_box.b_obj.o_obj.ob_pd, &s_signal, &s_signal);
//
//
//    ebox_ready((t_ebox *)x);
//
//    x->e_box.b_boxparameters.d_boxfillcolor = rgba_greylight;
//    x->e_box.b_boxparameters.d_bordercolor = rgba_green;
//
//    //x->out1 = outlet_new((t_object*)x, &s_anything);
//
//    if (x->instance)
//        x->instance->addMethod(x->sig_name, 0);
//
//    return static_cast<void*>(x);
//
//}
//
//static void exp_signal_out_free(t_exp_signal* x, t_symbol*id, int argc, t_atom* argv)
//{
//    if (x->instance)
//        x->instance->freeMethod(x->sig_name);
//
//}
//
//static void exp_signalout_any(t_exp_signal* x, t_symbol*id, int argc, t_atom* argv)
//{
//    if (x->instance)
//    {
//        AtomList list((Atom(id)));
//        AtomList list2(argc,argv);
//        list.append(list2);
//        if (x->instance)
//            x->instance->multipleOutput(list);
//
//    }
//}

#pragma mark -

extern "C" void setup_exp0x2esignal_tilde()
{

    exp_signal_class = eclass_new((OBJ_NAME),
        reinterpret_cast<t_typ_method>(exp_signal_new),
        reinterpret_cast<t_typ_method>(exp_signal_free),
        sizeof(t_exp_signal), CLASS_NOINLET, A_GIMME, 0);

    eclass_dspinit(exp_signal_class);

    eclass_addmethod(exp_signal_class, (t_typ_method)exp_signal_dsp, "dsp", A_NULL, 0);
    
    //eclass_addmethod(exp_instance_class, (t_typ_method)exp_instance_dsp, ("dsp"), A_NULL, 0);


    //CLASS_MAINSIGNALIN((t_class*)exp_signal_class, t_exp_signal, in_f);

    eclass_register(CLASS_OBJ, exp_signal_class);
}

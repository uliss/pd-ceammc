
/*

————————————————————————————————————————————————————————————————
a - torus © andré sier 20030914
		
		distribuicao torusa

————————————————————————————————————————————————————————————————
*/

// CEAMMC pd library version

#include "m_pd.h"

#include <math.h>
#include <stdbool.h>

#define pi 3.14159265358979323846264338327950288419716939937510f
#define twopi 2 * pi

typedef struct
{
    t_object x_obj;
    t_outlet* c_out;
    t_outlet* c_out2;

    double x0, y0, cr;
    double x0init, y0init, crinit;

    bool om;
} torus;

void* torus_new(t_symbol* msg, short argc, t_atom* argv);
void torus_bang(torus* x);
void torus_calc(torus* x);
void torus_set(torus* x, t_symbol* msg, short argc, t_atom* argv);
void torus_reset(torus* x, t_symbol* msg, short argc, t_atom* argv);

void torus_x0(torus* x, float echo);
void torus_y0(torus* x, float echo);
void torus_cr(torus* x, float echo);

void torus_assist(torus* x, void* b, long m, long a, char* s);
static t_class* torus_class;

void* torus_new(t_symbol* msg, short argc, t_atom* argv)
{
    torus* x;
    int i;

    x = (torus*)pd_new(torus_class);

    x->c_out2 = outlet_new(&x->x_obj, &s_float);
    x->c_out = outlet_new(&x->x_obj, &s_float);

    x->x0 = 0.777f;
    x->y0 = 1.2f;
    x->cr = 1.26f;
    x->x0init = 0.777f;
    x->y0init = 1.2f;
    x->crinit = 1.26f;

    x->om = 0;

    torus_set(x, msg, argc, argv);

    return (x);
}

void torus_set(torus* x, t_symbol* msg, short argc, t_atom* argv) //input the args
{

    if (argc) {

        if (argc > 2) {
            if (argv[2].a_type == A_FLOAT)
                x->crinit = argv[2].a_w.w_float;
            if (argv[2].a_type == A_FLOAT)
                x->crinit = (double)argv[2].a_w.w_float;
            x->cr = x->crinit;
        }
        if (argc > 1) {
            if (argv[1].a_type == A_FLOAT)
                x->y0init = argv[1].a_w.w_float;
            if (argv[1].a_type == A_FLOAT)
                x->y0init = (double)argv[1].a_w.w_float;
            x->y0 = x->y0init;
        }
        if (argc > 0) {
            if (argv[0].a_type == A_FLOAT)
                x->x0init = argv[0].a_w.w_float;
            if (argv[0].a_type == A_FLOAT)
                x->x0init = (double)argv[0].a_w.w_float;

            x->x0 = x->x0init;
        }

    } //end if args
}

void torus_reset(torus* x, t_symbol* msg, short argc, t_atom* argv)
{
    x->cr = x->crinit;
    x->x0 = x->x0init;
    x->y0 = x->y0init;
}

void torus_calc(torus* x)
{
    double x0, y0, cr;

    x0 = x->x0;
    y0 = x->y0;
    cr = x->cr;

    x->x0 = fmod((x0 + cr * sin(y0)), twopi);
    x->y0 = fmod((x0 + y0), twopi);
}

void torus_om(torus* x, int n) { x->om = (n > 0); }

void torus_bang(torus* x)
{

    outlet_float(x->c_out2, x->y0);
    outlet_float(x->c_out, x->x0);

    torus_calc(x); //next
}

void torus_cr(torus* x, float echo)
{
    x->cr = echo;
    if (x->om)
        torus_bang(x);
}
void torus_x0(torus* x, float echo)
{
    x->x0 = echo;
    if (x->om)
        torus_bang(x);
}
void torus_y0(torus* x, float echo)
{
    x->y0 = echo;
    if (x->om)
        torus_bang(x);
}

void torus_free() {}

void setup_noise0x2etorus()
{

    torus_class = class_new(gensym("noise.torus"),
        (t_newmethod)(torus_new),
        (t_method)(torus_free),
        sizeof(torus), 0, A_GIMME, 0);

    class_addbang(torus_class, (t_method)torus_bang);

    class_addmethod(torus_class, (t_method)torus_reset, gensym("reset"), A_GIMME, 0);
    class_addmethod(torus_class, (t_method)torus_set, gensym("set"), A_GIMME, 0);
    class_addmethod(torus_class, (t_method)torus_cr, gensym("cr"), A_DEFFLOAT, 0);
    class_addmethod(torus_class, (t_method)torus_x0, gensym("x"), A_DEFFLOAT, 0);
    class_addmethod(torus_class, (t_method)torus_y0, gensym("y"), A_DEFFLOAT, 0);
    class_addmethod(torus_class, (t_method)torus_om, gensym("om"), A_DEFFLOAT, 0);
}



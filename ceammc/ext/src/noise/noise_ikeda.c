/*
Ikeda attractor

 zn+1 = a + b zn exp[ i k - i p / (1 + | zn2 | ) ]

where z is a complex number

real number formula by euler returns:

xn+1 = a + k*( x0*cos(b-(p/(1+x0*x0+y0*y0))) - y0*sin(b-(p/(1*x0*x0+y0*y0))) )
yn+1 =     k*( x0*sin(b-(p/(1+x0*x0+y0*y0))) - y0*cos(b-(p/(1*x0*x0+y0*y0))) )


 a = 0.85, b = 0.9, k = 0.4, p = 7.7
 a = 1.0, b = 0.9, k = 0.4, p = 6.0
*/

// CEAMMC pd library version

#include "m_pd.h"

#include <math.h>
#include <stdbool.h>

typedef struct
{
    t_object x_obj;
    t_outlet* c_out; // value
    t_outlet* c_out2; // value

    double a, b, k, p, nx, ny; //eq variables
    double ainit, binit, kinit, pinit, nxinit, nyinit; //init eq variables

    bool om;
} ikeda;

void* ikeda_new(t_symbol* msg, short argc, t_atom* argv);
void ikeda_bang(ikeda* x);

//user variables
void ikeda_a(ikeda* x, double echo);
void ikeda_b(ikeda* x, double echo);
void ikeda_k(ikeda* x, double echo);
void ikeda_nx(ikeda* x, double echo);
void ikeda_ny(ikeda* x, double echo);
void ikeda_p(ikeda* x, double echo);

//calc
void ikeda_calc(ikeda* x);
void ikeda_reset(ikeda* x, t_symbol* msg, short argc, t_atom* argv);
void ikeda_set(ikeda* x, t_symbol* msg, short argc, t_atom* argv);

void ikeda_assist(ikeda* x, void* b, long m, long a, char* s);
static t_class* ikeda_class;

void* ikeda_new(t_symbol* msg, short argc, t_atom* argv) //input the args
{
    ikeda* x;
    int i;

    x = (ikeda*)pd_new(ikeda_class);

    x->c_out2 = outlet_new(&x->x_obj, &s_float);
    x->c_out = outlet_new(&x->x_obj, &s_float);

    //classic ikeda  a = 0.85, b = 0.9, k = 0.4, p = 7.7

    x->a = 0.85;
    x->b = 0.9;
    x->k = 0.4;
    x->p = 7.7;
    x->nx = 0;
    x->ny = 0;

    x->ainit = 0.85;
    x->binit = 0.9;
    x->kinit = 0.4;
    x->pinit = 7.7;
    x->nxinit = 0;
    x->nyinit = 0;

    x->om = 0;

    ikeda_set(x, msg, argc, argv);

    return (x);
}

void ikeda_om(ikeda* x, int n) { x->om = (n > 0); }

void ikeda_set(ikeda* x, t_symbol* msg, short ac, t_atom* av) //input the args
{
    if (ac) {

        if (ac > 5) {
            if (av[5].a_type == A_FLOAT)
                x->pinit = (double)av[5].a_w.w_float;
            else if (av[5].a_type == A_FLOAT)
                x->pinit = av[5].a_w.w_float;
            x->p = x->pinit;
        }
        if (ac > 4) {
            if (av[4].a_type == A_FLOAT)
                x->kinit = (double)av[4].a_w.w_float;
            else if (av[4].a_type == A_FLOAT)
                x->kinit = av[4].a_w.w_float;
            x->k = x->kinit;
        }
        if (ac > 3) {
            if (av[3].a_type == A_FLOAT)
                x->binit = (double)av[3].a_w.w_float;
            else if (av[3].a_type == A_FLOAT)
                x->binit = av[3].a_w.w_float;
            x->b = x->binit;
        }
        if (ac > 2) {
            if (av[2].a_type == A_FLOAT)
                x->ainit = (double)av[2].a_w.w_float;
            else if (av[2].a_type == A_FLOAT)
                x->ainit = av[2].a_w.w_float;
            x->a = x->ainit;
        }
        if (ac > 1) {
            if (av[1].a_type == A_FLOAT)
                x->nyinit = (double)av[1].a_w.w_float;
            else if (av[1].a_type == A_FLOAT)
                x->nyinit = av[1].a_w.w_float;
            x->ny = x->nyinit;
        }
        if (ac > 0) {
            if (av[0].a_type == A_FLOAT)
                x->nxinit = (double)av[0].a_w.w_float;
            else if (av[0].a_type == A_FLOAT)
                x->nxinit = av[0].a_w.w_float;
            x->nx = x->nxinit;
        }

    } // end if args
}

void ikeda_reset(ikeda* x, t_symbol* msg, short argc, t_atom* argv)
{
    x->nx = x->nxinit;
    x->ny = x->nyinit;
    x->a = x->ainit;
    x->b = x->binit;
    x->k = x->kinit;
    x->p = x->pinit;
}

void ikeda_calc(ikeda* x)
{
    double a, b, k, p, x0, y0;
    double sinx, cosx, temp;

    //local copies
    a = x->a;
    b = x->b;
    k = x->k;
    p = x->p;
    x0 = x->nx;
    y0 = x->ny;

    temp = (p / (1 + x0 * x0 + y0 * y0));
    cosx = (cos(b - temp));
    sinx = (sin(b - temp));
    /*
xn+1 = a + k*( x0*cos(b-(p/(1+x0*x0+y0*y0))) - y0*sin(b-(p/(1*x0*x0+y0*y0))) )
yn+1 =     k*( x0*sin(b-(p/(1+x0*x0+y0*y0))) - y0*cos(b-(p/(1*x0*x0+y0*y0))) )
*/
    x->nx = a + k * (x0 * cosx) - y0 * sinx;
    x->ny = k * sinx - y0 * cosx;
}

void ikeda_a(ikeda* x, double echo)
{
    x->a = (double)echo;
    //	x->ainit = (double)echo;
    if (x->om)
        ikeda_bang(x);
}

void ikeda_b(ikeda* x, double echo)
{
    x->b = (double)echo;
    //	x->rinit = (double)echo;
    if (x->om)
        ikeda_bang(x);
}

void ikeda_k(ikeda* x, double echo)
{
    x->k = (double)echo;
    //	x->cinit = (double)echo;
    if (x->om)
        ikeda_bang(x);
}

void ikeda_nx(ikeda* x, double echo)
{
    x->nx = (double)echo;
    //	x->nxinit = (double)echo;
    if (x->om)
        ikeda_bang(x);
}

void ikeda_ny(ikeda* x, double echo)
{
    x->ny = (double)echo;
    //	x->nyinit = (double)echo;
    if (x->om)
        ikeda_bang(x);
}

void ikeda_p(ikeda* x, double echo)
{
    x->p = (double)echo;
    //	x->nzinit = (double)echo;
    if (x->om)
        ikeda_bang(x);
}

void ikeda_bang(ikeda* x)
{

    outlet_float(x->c_out2, x->ny);
    outlet_float(x->c_out, x->nx);

    ikeda_calc(x); //next
}

void ikeda_free() {}

void setup_noise0x2eikeda()
{

    ikeda_class = class_new(gensym("noise.ikeda"),
        (t_newmethod)(ikeda_new),
        (t_method)(ikeda_free),
        sizeof(ikeda), 0, A_GIMME, 0);

    class_addbang(ikeda_class, (t_method)ikeda_bang);
    class_addmethod(ikeda_class, (t_method)ikeda_set, gensym("set"), A_GIMME, 0);
    class_addmethod(ikeda_class, (t_method)ikeda_reset, gensym("reset"), A_GIMME, 0);

    class_addmethod(ikeda_class, (t_method)ikeda_nx, gensym("x"), A_DEFFLOAT, 0);
    class_addmethod(ikeda_class, (t_method)ikeda_ny, gensym("y"), A_DEFFLOAT, 0);
    class_addmethod(ikeda_class, (t_method)ikeda_a, gensym("a"), A_DEFFLOAT, 0);
    class_addmethod(ikeda_class, (t_method)ikeda_b, gensym("b"), A_DEFFLOAT, 0);
    class_addmethod(ikeda_class, (t_method)ikeda_k, gensym("c"), A_DEFFLOAT, 0);
    class_addmethod(ikeda_class, (t_method)ikeda_p, gensym("p"), A_DEFFLOAT, 0);
    class_addmethod(ikeda_class, (t_method)ikeda_om, gensym("om"), A_DEFFLOAT, 0);
}



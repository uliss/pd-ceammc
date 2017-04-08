/*

————————————————————————————————————————————————————————————————
a-henon - © andré sier 2004

updated to use init vars method ported from chaos collection		
————————————————————————————————————————————————————————————————
*/

// CEAMMC pd library version

#include "cicm_wrapper.h"

#include <math.h>
#include <stdbool.h>

typedef struct _henon {
    t_object c_ob;
    void *c_out, *c_out2;
    double a, b, nx, ny;
    double ainit, binit, nxinit, nyinit;

    bool om;
} henon;

void* henon_new(t_symbol* msg, short argc, t_atom* argv);
void henon_calc(henon* x);
void henon_bang(henon* x);
void henon_set(henon* x, t_symbol* msg, short argc, t_atom* argv);
void henon_reset(henon* x, t_symbol* msg, short argc, t_atom* argv);

void henon_a(henon* x, double max);
void henon_b(henon* x, double max);
void henon_om(henon* x, long max);

void henon_assist(henon* x, void* b, long m, long a, char* s);
static t_eclass* henon_class;

void* henon_new(t_symbol* msg, short argc, t_atom* argv) //input the args
{
    henon* x;
    int i;

    x = (henon*)eobj_new(henon_class);

    x->c_out2 = floatout(x);
    x->c_out = floatout(x);

    //init
    x->a = 1.4f;
    x->b = 0.3f;
    x->nx = 1.0f;
    x->ny = 1.0f;

    x->ainit = 1.4f;
    x->binit = 0.3f;
    x->nxinit = 1.0f;
    x->nyinit = 1.0f;

    if (argc)
        henon_set(x, msg, argc, argv);

    return (x);
}

void henon_set(henon* x, t_symbol* s, short ac, t_atom* av) //input the args
{

    if (ac) {

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

    } //end if args
}

void henon_reset(henon* x, t_symbol* s, short ac, t_atom* av) //input the args
{
    x->a = x->ainit;
    x->b = x->binit;
    x->nx = x->nxinit;
    x->ny = x->nyinit;
}
void henon_calc(henon* x)
{
    double nx, ny, lx1, ly1;
    double a, b;

    a = x->a;
    b = x->b;
    nx = x->nx;
    ny = x->ny;

    x->nx = (ny + 1) - (a * (nx * nx));
    x->ny = b * nx;
}

void henon_bang(henon* x)
{

    outlet_float(x->c_out2, x->ny);
    outlet_float(x->c_out, x->nx);

    henon_calc(x);
}

void henon_a(henon* x, double max)
{
    x->a = max;
    if (x->om)
        henon_bang(x);
}
void henon_b(henon* x, double max)
{
    x->b = max;
    if (x->om)
        henon_bang(x);
}
void henon_nx(henon* x, double max)
{
    x->nx = max;
    if (x->om)
        henon_bang(x);
}
void henon_ny(henon* x, double max)
{
    x->ny = max;
    if (x->om)
        henon_bang(x);
}

void henon_om(henon* x, long max) { x->om = (max > 0); }

void henon_free() {}

void setup_noise0x2ehenon()
{

    henon_class = eclass_new(("noise.henon"),
        (t_typ_method)(henon_new),
        (t_typ_method)(henon_free),
        sizeof(henon), 0, A_GIMME, 0);

    eclass_addmethod(henon_class, (method)henon_bang, "bang", A_GIMME, 0);
    eclass_addmethod(henon_class, (method)henon_set, "set", A_GIMME, 0);
    eclass_addmethod(henon_class, (method)henon_reset, "reset", A_GIMME, 0);
    eclass_addmethod(henon_class, (method)henon_a, "a", A_FLOAT, 0);
    eclass_addmethod(henon_class, (method)henon_b, "b", A_FLOAT, 0);
    eclass_addmethod(henon_class, (method)henon_nx, "x", A_FLOAT, 0);
    eclass_addmethod(henon_class, (method)henon_ny, "y", A_FLOAT, 0);
    eclass_addmethod(henon_class, (method)henon_om, "om", A_FLOAT, 0);
}



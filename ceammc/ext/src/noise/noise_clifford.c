/*

————————————————————————————————————————————————————————————————
a-clifford - © andré sier 2004
	
inspired by Paul Bourke, implementation by André Sier	
		
clifford Attractors
 Attributed to Cliff Pickover

 Graphics by Paul Bourke
 February 2004

 See also Peter de Jong attractors 
 xn+1 = sin(a yn) + c cos(a xn)
 yn+1 = sin(b xn) + d cos(b yn)
 where a, b, c, d are variabies that define each attractor.

 a = -1.4, b = 1.6, c = 1.0, d = 0.7 
 a = 1.1, b = -1.0, c = 1.0, d = 1.5
 a = 1.6, b = -0.6, c = -1.2, d = 1.6
 a = 1.7, b = 1.7, c = 0.06, d = 1.2
 a = 1.3, b = 1.7, c = 0.5, d = 1.4
 
 
 vers 1.0.2
 
 updated init variables & calc equation & set methods & reset & info

————————————————————————————————————————————————————————————————
*/

// CEAMMC pd library version

#include "cicm_wrapper.h"

#include <math.h>
#include <stdbool.h>

typedef struct _clifford {
    t_eobj c_ob;
    t_outlet *c_out1, *c_out2; // outlets
    double a, b, c, d, nx, ny;
    double ainit, binit, cinit, dinit, nxinit, nyinit;

    bool om;
} clifford;

static inline double clif_calc(double a, double b, double c, double d)
{
    return sin(a * b) + c * cos(a * d);
}

static void* clifford_new(t_symbol* msg, int argc, t_atom* argv);
static void clifford_calc(clifford* x);
static void clifford_bang(clifford* x);
static void clifford_set(clifford* x, t_symbol* msg, int argc, t_atom* argv);
static void clifford_reset(clifford* x);

static void clifford_a(clifford* x, t_float max);
static void clifford_b(clifford* x, t_float max);
static void clifford_c(clifford* x, t_float max);
static void clifford_d(clifford* x, t_float max);
static void clifford_nx(clifford* x, t_float max);
static void clifford_ny(clifford* x, t_float max);
static void clifford_om(clifford* x, t_float max);

static t_eclass* clifford_class;

void* clifford_new(t_symbol* msg, int argc, t_atom* argv)
{
    clifford* x;

    x = (clifford*)eobj_new(clifford_class);

    x->c_out2 = floatout(x);
    x->c_out1 = floatout(x);

    //init
    x->a = -1.4;
    x->ainit = -1.4;
    x->b = 1.6;
    x->binit = 1.6;
    x->c = 1.0;
    x->cinit = 1.0;
    x->d = 0.7;
    x->dinit = 0.7;
    // do not init x and y for more chaotic results
    x->nx = 0;
    x->nxinit = 0;
    x->ny = 0;
    x->nyinit = 0;

    x->om = 0;

    if (argc)
        clifford_set(x, msg, argc, argv);

    return (x);
}

void clifford_free(clifford* x)
{
    outlet_free(x->c_out1);
    outlet_free(x->c_out2);
}

void clifford_set(clifford* x, t_symbol* msg, int argc, t_atom* argv) //input the args
{
    //supa elegant xoaz set method
    if (argc) {
        if (argc > 5) {
            if (argv[5].a_type == A_FLOAT)
                x->nyinit = atom_getfloat(&argv[5]);
            x->ny = x->nyinit;
        }

        if (argc > 4) {
            if (argv[4].a_type == A_FLOAT)
                x->nxinit = atom_getfloat(&argv[4]);
            x->nx = x->nxinit;
        }

        if (argc > 3) {
            if (argv[3].a_type == A_FLOAT)
                x->dinit = atom_getfloat(&argv[3]);
            x->d = x->dinit;
        }

        if (argc > 2) {
            if (argv[2].a_type == A_FLOAT)
                x->cinit = atom_getfloat(&argv[2]);
            x->c = x->cinit;
        }

        if (argc > 1) {
            if (argv[1].a_type == A_FLOAT)
                x->binit = atom_getfloat(&argv[1]);
            x->b = x->binit;
        }

        if (argc > 0) {
            if (argv[0].a_type == A_FLOAT)
                x->ainit = atom_getfloat(&argv[0]);
            x->a = x->ainit;
        }

    } //end if args
}

void clifford_calc(clifford* x)
{
    double nx, ny;
    double a, b, c, d;

    a = x->a;
    b = x->b;
    c = x->c;
    d = x->d;
    nx = x->nx;
    ny = x->ny;

    // clifford attractor

    //	lx1 = sin (a*ny) + c*cos(a*nx);
    //	ly1 = sin (b*nx) + d*cos(b*ny);

    x->nx = clif_calc(a, ny, c, nx);
    x->ny = clif_calc(b, nx, d, ny);
}

void clifford_bang(clifford* x) //important, first output values then calc next
{
    outlet_float(x->c_out2, x->nx);
    outlet_float(x->c_out1, x->ny);
    clifford_calc(x);
}

void clifford_nx(clifford* x, t_float max)
{
    x->nx = max;

    if (x->om)
        clifford_bang(x);
}

void clifford_ny(clifford* x, t_float max)
{
    x->ny = max;

    if (x->om)
        clifford_bang(x);
}

void clifford_a(clifford* x, t_float max)
{
    x->a = max;

    if (x->om)
        clifford_bang(x);
}

void clifford_b(clifford* x, t_float max)
{
    x->b = max;

    if (x->om)
        clifford_bang(x);
}

void clifford_c(clifford* x, t_float max)
{
    x->c = max;

    if (x->om)
        clifford_bang(x);
}

void clifford_d(clifford* x, t_float max)
{
    x->d = max;

    if (x->om)
        clifford_bang(x);
}

void clifford_om(clifford* x, t_float max)
{
    x->om = (max > 0);
}

void clifford_reset(clifford* x)
{
    x->a = x->ainit;
    x->b = x->binit;
    x->c = x->cinit;
    x->d = x->dinit;
    x->nx = x->nxinit;
    x->ny = x->nyinit;
}

//void clifford_info(clifford* x, t_symbol* msg, short argc, t_atom* argv)
//{
//    post("__________________________________");
//    post("A-Chaos Lib::a-clifford info 102");
//    post("(a,b,c,d,x,y)init variables: %f, %f, %f, %f, %f, %f", x->ainit, x->binit, x->cinit, x->dinit, x->nxinit, x->nyinit, 0);
//    post("(a,b,c,d,x,y) variables: %f, %f, %f, %f, %f, %f", x->a, x->b, x->c, x->d, x->nx, x->ny, 0);
//    post("__________________________________");
//}

void setup_noise0x2eclifford()
{

    clifford_class = eclass_new(("noise.clifford"),
        (t_typ_method)(clifford_new),
        (t_typ_method)(clifford_free),
        sizeof(clifford), 0, A_GIMME, 0);

    eclass_addmethod(clifford_class, (method)clifford_bang, "bang", A_NULL, 0);
    eclass_addmethod(clifford_class, (method)clifford_reset, "reset", A_NULL, 0);
    eclass_addmethod(clifford_class, (method)clifford_set, "set", A_GIMME, 0);
    eclass_addmethod(clifford_class, (method)clifford_nx, "x", A_DEFFLOAT, 0);
    eclass_addmethod(clifford_class, (method)clifford_ny, "y", A_DEFFLOAT, 0);
    eclass_addmethod(clifford_class, (method)clifford_a, "a", A_DEFFLOAT, 0);
    eclass_addmethod(clifford_class, (method)clifford_b, "b", A_DEFFLOAT, 0);
    eclass_addmethod(clifford_class, (method)clifford_c, "c", A_DEFFLOAT, 0);
    eclass_addmethod(clifford_class, (method)clifford_d, "d", A_DEFFLOAT, 0);
    eclass_addmethod(clifford_class, (method)clifford_om, "om", A_DEFFLOAT, 0);
}

/*

————————————————————————————————————————————————————————————————
a-jong - © andré sier 2004

 vers 1.0.2
 
 
 
  See also Peter de Jong attractors 
 xn+1 = sin(a yn) + c cos(a xn)
 yn+1 = sin(b xn) + d cos(b yn)
 where a, b, c, d are variabies that define each attractor.

 a = -1.4, b = 1.6, c = 1.0, d = 0.7 
 a = 1.1, b = -1.0, c = 1.0, d = 1.5
 a = 1.6, b = -0.6, c = -1.2, d = 1.6
 a = 1.7, b = 1.7, c = 0.06, d = 1.2
 a = 1.3, b = 1.7, c = 0.5, d = 1.4
 
 
 
 updated init variables & calc equation & set methods & reset & info
	
————————————————————————————————————————————————————————————————
*/

// CEAMMC pd library version

#include "cicm_wrapper.h"

#include <math.h>
#include <stdbool.h>

//#define	j_calc(a, b, c, d) (sin((a)*(b))-cos((c)*(d)))

typedef struct _jong {
    t_object c_ob;
    void *c_out, *c_out2; // outlet
    double a, b, c, d, nx, ny;
    double ainit, binit, cinit, dinit, nxinit, nyinit;

    bool om;
} jong;

void* jong_new(t_symbol* msg, short argc, t_atom* argv);
void jong_calc(jong* x);
void jong_bang(jong* x);
void jong_set(jong* x, t_symbol* msg, short argc, t_atom* argv);
void jong_reset(jong* x, t_symbol* msg, short argc, t_atom* argv);
void jong_info(jong* x, t_symbol* msg, short argc, t_atom* argv);

void jong_a(jong* x, double max);
void jong_b(jong* x, double max);
void jong_c(jong* x, double max);
void jong_d(jong* x, double max);
void jong_nx(jong* x, double max);
void jong_ny(jong* x, double max);
void jong_om(jong* x, long max);

void jong_assist(jong* x, void* b, long m, long a, char* s);
static t_eclass* jong_class;

void* jong_new(t_symbol* msg, short argc, t_atom* argv) //input the args
{
    jong* x;
    int i;

    x = (jong*)eobj_new(jong_class);

    x->c_out2 = floatout(x);
    x->c_out = floatout(x);

    //init
    x->a = 1.4f;
    x->b = -2.3f;
    x->c = 2.4f;
    x->d = -2.1f;
    x->nx = 0.0f;
    x->ny = 0.0f;
    x->ainit = 1.4f;
    x->binit = -2.3f;
    x->cinit = 2.4f;
    x->dinit = -2.1f;
    x->nxinit = 0.0f;
    x->nyinit = 0.0f;

    x->om = 0;

    if (argc)
        jong_set(x, msg, argc, argv);

    return (x);
}

void jong_om(jong* x, long max) { x->om = (max > 0); }

void jong_set(jong* x, t_symbol* msg, short argc, t_atom* argv) //input the args
{
    // super elegant xoaz set method
    if (argc) {
        if (argc > 5) {
            if (argv[5].a_type == A_FLOAT)
                x->nyinit = (double)argv[5].a_w.w_float;
            else if (argv[5].a_type == A_FLOAT)
                x->nyinit = (double)argv[5].a_w.w_float;
            x->ny = x->nyinit;
        }

        if (argc > 4) {
            if (argv[4].a_type == A_FLOAT)
                x->nxinit = (double)argv[4].a_w.w_float;
            else if (argv[4].a_type == A_FLOAT)
                x->nyinit = (double)argv[4].a_w.w_float;
            x->nx = x->nxinit;
        }

        if (argc > 3) {
            if (argv[3].a_type == A_FLOAT)
                x->dinit = (double)argv[3].a_w.w_float;
            else if (argv[3].a_type == A_FLOAT)
                x->dinit = (double)argv[3].a_w.w_float;
            x->d = x->dinit;
        }

        if (argc > 2) {
            if (argv[2].a_type == A_FLOAT)
                x->cinit = (double)argv[2].a_w.w_float;
            else if (argv[2].a_type == A_FLOAT)
                x->cinit = (double)argv[2].a_w.w_float;
            x->c = x->cinit;
        }

        if (argc > 1) {
            if (argv[1].a_type == A_FLOAT)
                x->binit = (double)argv[1].a_w.w_float;
            else if (argv[1].a_type == A_FLOAT)
                x->binit = (double)argv[1].a_w.w_float;
            x->b = x->binit;
        }

        if (argc > 0) {
            if (argv[0].a_type == A_FLOAT)
                x->ainit = (double)argv[0].a_w.w_float;
            else if (argv[0].a_type == A_FLOAT)
                x->ainit = (double)argv[0].a_w.w_float;
            x->a = x->ainit;
        }

        //	if (x->om) jong_bang(x);
        // this is a set method
    } //end if args
}

void jong_calc(jong* x)
{
    double nx, ny;
    double a, b, c, d;

    a = x->a;
    b = x->b;
    c = x->c;
    d = x->d;
    nx = x->nx;
    ny = x->ny;

    // Peter de Jong attractor
    //	lx1 = sin(a*ny)-cos(b*nx);
    //	ly1 = sin(c*nx)-cos(d*ny);

    x->nx = sin(a * ny) - cos(b * nx);
    x->ny = sin(c * nx) - cos(d * ny);
}

void jong_bang(jong* x) //important, first output values then calc next
{
    outlet_float(x->c_out2, x->nx);
    outlet_float(x->c_out, x->ny);

    jong_calc(x);
}

void jong_nx(jong* x, double max)
{
    x->nx = max;
    //	x->nxinit = max;
    if (x->om)
        jong_bang(x);
}
void jong_ny(jong* x, double max)
{
    x->ny = max;
    //	x->nyinit = max;
    if (x->om)
        jong_bang(x);
}
void jong_a(jong* x, double max)
{
    x->a = max;
    //	x->ainit = max;
    if (x->om)
        jong_bang(x);
}
void jong_b(jong* x, double max)
{
    x->b = max;
    //	x->binit = max;
    if (x->om)
        jong_bang(x);
}
void jong_c(jong* x, double max)
{
    x->c = max;
    //	x->cinit = max;
    if (x->om)
        jong_bang(x);
}
void jong_d(jong* x, double max)
{
    x->d = max;
    //	x->dinit = max;
    if (x->om)
        jong_bang(x);
}

void jong_reset(jong* x, t_symbol* msg, short argc, t_atom* argv)
{
    x->a = x->ainit;
    x->b = x->binit;
    x->c = x->cinit;
    x->d = x->dinit;
    x->nx = x->nxinit;
    x->ny = x->nyinit;
}

void jong_free() {}

void setup_noise0x2ejong()
{

    jong_class = eclass_new(("noise.jong"),
        (t_typ_method)(jong_new),
        (t_typ_method)(jong_free),
        sizeof(jong), 0, A_GIMME, 0);

    eclass_addmethod(jong_class, (method)jong_bang, "bang", A_GIMME, 0);
    eclass_addmethod(jong_class, (method)jong_reset, "reset", A_GIMME, 0);
    eclass_addmethod(jong_class, (method)jong_set, "set", A_GIMME, 0);
    eclass_addmethod(jong_class, (method)jong_nx, "x", A_DEFFLOAT, 0);
    eclass_addmethod(jong_class, (method)jong_ny, "y", A_DEFFLOAT, 0);
    eclass_addmethod(jong_class, (method)jong_a, "a", A_DEFFLOAT, 0);
    eclass_addmethod(jong_class, (method)jong_b, "b", A_DEFFLOAT, 0);
    eclass_addmethod(jong_class, (method)jong_c, "c", A_DEFFLOAT, 0);
    eclass_addmethod(jong_class, (method)jong_d, "d", A_DEFFLOAT, 0);
    eclass_addmethod(jong_class, (method)jong_om, "om", A_DEFFLOAT, 0);
}



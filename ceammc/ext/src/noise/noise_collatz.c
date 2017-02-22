/*

————————————————————————————————————————————————————————————————
collatz - © Copyright sier 2002
		
		collatz conjecture output
————————————————————————————————————————————————————————————————
*/

#include "cicm_wrapper.h"

#include <math.h>
#include <stdbool.h>

typedef struct
{
    t_object c_ob;
    void* c_out;
    long c_value;
    long c_offset;
    bool c_mode, om;
} atz;

void* atz_new(long inputarg, long offset);
void atz_offset(atz* x, long offset);
void atz_bang(atz* x);
void atz_int(atz* x, long number);
void atz_mode(atz* x, long number);
void atz_om(atz* x, long number);
int atz_calc(atz* x, int number);
//void atz_assist(atz* x, void* b, long m, long a, char* s);

t_eclass* atz_class;

void atz_mode(atz* x, long number)
{
    x->c_mode = number;
    // ?
    //    if (atz_om)
    //        atz_bang(x);
}

void* atz_new(long inputarg, long offset)
{
    atz* x;
    x = (atz*)eobj_new(atz_class);
    x->c_out = floatout(x);

    x->c_value = x->c_offset = 0; //init
    x->c_mode = x->om = 0;
    x->c_value = inputarg;
    if (!inputarg)
        x->c_value = 71; //seed it
    x->c_offset = offset;
    if (!offset)
        x->c_offset = 0;
    return (x); ///the obj, nice and clean, ready to use
}

int atz_calc(atz* x, int number)
{
    short int stub;
    stub = number % 2; //par ou impar

    if (!x->c_mode) { // L.Collatz collatz mode
        if (!stub)
            number /= 2; //par ]e metade
        else
            number = 3 * number + 1; //impar ]e 3n+1
        return (number);
    } else if (x->c_mode) { // Terras collatz mode
        if (!stub)
            number /= 2; //par ]e metade
        else
            number = 0.5 * (3 * number + 1); //impar ]e 1/2(3n+1)
        return (number);
    }

    // ?
    return -1;
}

void atz_offset(atz* x, long offset)
{
    x->c_offset = offset;
    // ?
    //if (atz_om)
    atz_bang(x);
}
void atz_om(atz* x, long number) { x->om = (number > 0); }

void atz_int(atz* x, long number)
{
    x->c_value = atz_calc(x, (int)number - 1); //guardar dentro do objecto calculado
    // ?
    //if (atz_om)
    atz_bang(x);
}

void atz_bang(atz* x)
{

    outlet_int(x->c_out, x->c_value + x->c_offset);

    x->c_value = atz_calc(x, (int)x->c_value); //run again
}

void atz_free() {}

void setup_noise0x2ecollatz()
{

    atz_class = eclass_new(("noise.collatz"),
        (t_typ_method)(atz_new),
        (t_typ_method)(atz_free),
        sizeof(atz), 0, A_GIMME, 0);

    //eclass_addmethod(lorenz_class, (method)baker_bang, "bang", A_GIMME, 0);

    eclass_addmethod(atz_class, (method)atz_bang, "bang", A_GIMME, 0);
    eclass_addmethod(atz_class, (method)atz_int, "float", A_GIMME, 0);
    eclass_addmethod(atz_class, (method)atz_offset, "offset", A_DEFFLOAT, 0);
    eclass_addmethod(atz_class, (method)atz_mode, "mode", A_DEFFLOAT, 0);
    eclass_addmethod(atz_class, (method)atz_om, "om", A_DEFFLOAT, 0);
    
    post("noise.collatz: part of A-Chaos library, (C) 2004 André Sier");
}


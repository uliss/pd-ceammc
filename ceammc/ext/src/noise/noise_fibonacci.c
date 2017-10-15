/*
————————————————————————————————————————————————————————————————
Fibonacci - © Andr[e Sier 2002

revision 09.14.2002
	f0,f1 -- 1
	tst ()


output fibonacci series of numbers

modos:	raw (calcular a iteracao da sequencia fibo n)
		lookup table (calcular n valores da sequencia e armazenar numa tabela)		
————————————————————————————————————————————————————————————————
*/

// CEAMMC pd library version

#include "m_pd.h"

#include <math.h>
#include <stdbool.h>

#define MAXSIZE 256
//#define MOD 10000000

typedef struct _something_shines_every_day_twelve_oh_five {
    t_object x_ob;
    long x_a; // nada == swim / double.v
    long x_iter[MAXSIZE]; //a iteracao a calcular
    bool x_fire; //bang on calc?
    bool x_rep;
    void* x_outlet;
    long x_fill;
} fibo;

void* fibo_new(long arg);
void fibo_assist(fibo* x, void* b, long m, long a, char* s);
void fibo_bang(fibo* x);
void fibo_int(fibo* x, long n);
void fibo_fire(fibo* x, long n);
void fibo_rep(fibo* x, long n);
void fibo_free(fibo* x);
void fibo_tavle(fibo* x, long n);
void fibo_filltavle(fibo* x, long n);
void fibo_trust(fibo* x, long n);
int Fibonacci(int n);
t_class* fibo_class;

// 0 @ 47
long fivo[] = { 1, 1, 2, 3, 5, 8, 13, 21, 34, 55,
    89, 144, 233, 377, 610, 987, 1597, 2584, 4181, 6765,
    10946, 17711, 28657, 46368, 75025, 121393, 196418, 317811, 514229, 832040,
    1346269, 2178309, 3524578, 5702887, 9227465, 14930352, 24157817, 39088169, 63245986, 102334155,
    165580141, 267914296, 433494437, 701408733, 1134903170, 1836311903, -13233752223 };

void fibo_fire(fibo* x, long n) { x->x_fire = n; }
void fibo_rep(fibo* x, long n) { x->x_rep = n; }

void fibo_int(fibo* x, long n) //output n'th iteration , the further the more expensive
{
    x->x_a = Fibonacci(n);
    if (x->x_fire)
        fibo_bang(x);
}

void fibo_bang(fibo* x)
{
    outlet_int(x->x_outlet, x->x_a);
}

void fibo_tavle(fibo* x, long n)
{
    x->x_a = x->x_iter[n];
    if (x->x_fire)
        fibo_bang(x);
}

void fibo_filltavle(fibo* x, long n)
{
    register short int i;
    for (i = x->x_fill; i < (n + x->x_fill); i++)
        x->x_iter[i] = Fibonacci(i);
    x->x_fill += n;

    if (x->x_rep)
        post("fill is %ld", x->x_fill, 0);
}

void fibo_trust(fibo* x, long n)
{
    x->x_a = fivo[n];
    if (x->x_rep)
        post("&fill is %07x", &fivo[n], 0);
    if (x->x_fire)
        fibo_bang(x);
}

int Fibonacci(int n)
{
    if (n < 2) {
        if (n == 0 || n == 1)
            return 1; //f0,f1 =1
        else
            return (n); // no mirror
    } else
        return (Fibonacci(n - 1) + Fibonacci(n - 2));
}

void fibo_assist(fibo* x, void* b, long m, long a, char* s)
{
    if ((m == 1) && (a == 0))
        sprintf(s, "(int), tavle, bang");
    else if ((m == 2) && (a == 0))
        sprintf(s, "(int)fibo out");
}

void* fibo_new(long arg)
{
    register short i;

    fibo* x;
    x = (fibo*)pd_new(fibo_class);

    x->x_a = 0;
    x->x_fill = 0;

    for (i = 0; i < MAXSIZE; i++) // tavula redonda
        x->x_iter[i] = 0;

    if (arg)
        fibo_filltavle(x, arg);

    x->x_outlet = outlet_new(&x->x_obj, &s_float);
    x->x_fire = 1;
    x->x_rep = 0;
    return (x);
}

void fibo_free(fibo* x)
{
    // ?
    //notify_free((t_object*)x);
}

void setup_noise0x2efibonacci()
{

    fibo_class = class_new(gensym("noise.fibonacci"),
        (t_typ_method)(fibo_new),
        (t_typ_method)(fibo_free),
        sizeof(fibo), 0, A_GIMME, 0);

    //class_addmethod(lorenz_class, (t_method)baker_bang, "bang", A_GIMME, 0);

    class_addmethod(fibo_class, (t_method)fibo_bang, "bang", A_GIMME, 0);
    class_addmethod(fibo_class, (t_method)fibo_int, "float", A_GIMME, 0);
    class_addmethod(fibo_class, (t_method)fibo_fire, "fire", A_FLOAT, 0);
    class_addmethod(fibo_class, (t_method)fibo_tavle, "tavle", A_FLOAT, 0);
    class_addmethod(fibo_class, (t_method)fibo_trust, "tst", A_FLOAT, 0);
    class_addmethod(fibo_class, (t_method)fibo_filltavle, "fill", A_FLOAT, 0);
    class_addmethod(fibo_class, (t_method)fibo_rep, "rep", A_FLOAT, 0);
}


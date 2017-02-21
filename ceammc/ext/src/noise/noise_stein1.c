/*

ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ
a - stein1 © andrŽ sier 20030914
		
		distribuicao stein1a

ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ
*/

#include "ext.h"
#include "ext_common.h"

#include "math.h"

#define pie 3.14159265358979323846


typedef struct	
{
	t_object 	        c_ob;			
	void 		        *c_out;		
	
	double lambda, seed, seedinit; 
	Boolean om;
} stein1;


void *stein1_new (Symbol *msg, short argc, Atom *argv);
void stein1_bang (stein1 *x);
void stein1_calc (stein1 *x);
void stein1_set (stein1 *x, Symbol *msg, short argc, Atom *argv);  
void stein1_reset(stein1 *x, Symbol *msg, short argc, Atom *argv);

void stein1_echo (stein1 *x, float echo);
void stein1_int(stein1 *x, int n);
void stein1_float(stein1 *x, float n);

void stein1_lambda (stein1 *x, float echo);
void stein1_seed (stein1 *x, float echo);

void stein1_assist(stein1 *x, void *b, long m, long a, char *s);
void *stein1_class;


void *stein1_new (Symbol *msg, short argc, Atom *argv) //input the args 
{
	 stein1 *x;
	 int i;
	 
	 x=(stein1 *)newobject(stein1_class);

	 x->c_out=floatout(x);
	 
	x->seed = 0.87f;
	x->seedinit = 0.87f;
	x->lambda = 2.25f;
	x->om = 0;

	stein1_set(x, msg, argc, argv);

	 return(x);	
}

void stein1_set (stein1 *x, Symbol *msg, short argc, Atom *argv) //input the args 
{
	
	if (argc) {

		if (argc > 1) {
			if (argv[1].a_type == A_FLOAT)
				x->lambda = argv[1].a_w.w_float;
			if (argv[1].a_type == A_LONG)
				x->lambda = (double)argv[1].a_w.w_long;			
		}
		if (argc > 0) {
			if (argv[0].a_type == A_FLOAT)
				x->seedinit = argv[0].a_w.w_float;
			if (argv[0].a_type == A_LONG)
				x->seedinit = (double)argv[0].a_w.w_long;
				
			x->seed = x->seedinit;
		}

	} //end if args
}

void  stein1_reset (stein1 *x, Symbol *msg, short argc, Atom *argv)
{	
	x->seed = x->seedinit;
}


void  stein1_calc (stein1 *x)
{
	double lambda, seed ;
	
	lambda = x->lambda;
	seed = x->seed;
	
	x->seed = lambda * seed * seed * sin(pie*seed);

}



void stein1_echo (stein1 *x, float echo)
{
 	x->lambda = echo;
	if (x->om) stein1_bang(x);
}

void stein1_int(stein1 *x, int n)     { stein1_echo(x, n*0.01);}
void stein1_float(stein1 *x, float n) { stein1_echo(x, n);}


void stein1_om(stein1 *x, int n)     { x->om = (n>0);}


void stein1_bang (stein1 *x)
{

    outlet_float(x->c_out, x->seed); 
	
    stein1_calc(x);   //next
    
}

void stein1_lambda (stein1 *x, float echo)
{
	x->lambda = echo;
	if (x->om) stein1_bang(x);
}
void stein1_seed (stein1 *x, float echo)
{
	x->seed = echo;
	x->seedinit = echo;
	if (x->om) stein1_bang(x);
}


void main(void)
{
 long int tick = gettime();
 setup((t_messlist**)&stein1_class,(method)stein1_new,0L,(short)sizeof(stein1),0L,
 A_GIMME,0);

 addbang((method)stein1_bang);
 
 addmess((method)stein1_reset,"reset", A_GIMME, 0);
 addmess((method)stein1_set,"set", A_GIMME, 0);
 addint((method)stein1_int);
 addfloat((method)stein1_float);
 addmess((method)stein1_lambda,"lambda", A_DEFFLOAT, 0);
 addmess((method)stein1_seed,"seed", A_DEFFLOAT, 0);
 addmess((method)stein1_om,"om", A_DEFLONG, 0);
 
 addmess((method)stein1_assist,"assist", A_CANT, 0);
 post("A-Chaos Lib :: a-stein1  " __DATE__" "__TIME__"                                   ©   a n d r Ž s i e r   2 0 0 4   all rights reserved",tick, 0);
}


void stein1_assist(stein1 *x, void *b, long m, long a, char *s)
{
    if (m==1) { sprintf(s,"bang, (int), (float)"); }   
    else if (m==2&&a==0) { sprintf(s,"(float) stein1    e         c            h              o"); }
}



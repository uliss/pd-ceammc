/*

ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ
a - stein © andrŽ sier 20030914
		
		distribuicao steina

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
} stein;


void *stein_new (Symbol *msg, short argc, Atom *argv);
void stein_bang (stein *x);
void stein_calc (stein *x);
void stein_set (stein *x, Symbol *msg, short argc, Atom *argv);  
void stein_reset(stein *x, Symbol *msg, short argc, Atom *argv);

void stein_echo (stein *x, float echo);
void stein_int(stein *x, int n);
void stein_float(stein *x, float n);

void stein_lambda (stein *x, float echo);
void stein_seed (stein *x, float echo);

void stein_assist(stein *x, void *b, long m, long a, char *s);
void *stein_class;


void *stein_new (Symbol *msg, short argc, Atom *argv) //input the args 
{
	 stein *x;
	 int i;
	 
	 x=(stein *)newobject(stein_class);

	 x->c_out=floatout(x);
	 
	x->seed = 0.777f;
	x->seedinit = 0.777f;
	x->lambda = 1.7f;
	x->om = 0;

	stein_set(x, msg, argc, argv);

	 return(x);	
}

void stein_set (stein *x, Symbol *msg, short argc, Atom *argv) //input the args 
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

void  stein_reset (stein *x, Symbol *msg, short argc, Atom *argv)
{	
	x->seed = x->seedinit;
}


void  stein_calc (stein *x)
{
	double lambda, seed ;
	
	lambda = x->lambda;
	seed = x->seed;
	
	x->seed = lambda * sin(pie*seed);

}



void stein_echo (stein *x, float echo)
{
 	x->lambda = echo;
	if (x->om) stein_bang(x);
}

void stein_int(stein *x, int n)     { stein_echo(x, n*0.01);}
void stein_float(stein *x, float n) { stein_echo(x, n);}


void stein_om(stein *x, int n)     { x->om = (n>0);}


void stein_bang (stein *x)
{

    outlet_float(x->c_out, x->seed); 
	
    stein_calc(x);   //next
    
}

void stein_lambda (stein *x, float echo)
{
	x->lambda = echo;
	if (x->om) stein_bang(x);
}
void stein_seed (stein *x, float echo)
{
	x->seed = echo;
	x->seedinit = echo;
	if (x->om) stein_bang(x);
}


void main(void)
{
 long int tick = gettime();
 setup((t_messlist**)&stein_class,(method)stein_new,0L,(short)sizeof(stein),0L,
 A_GIMME,0);

 addbang((method)stein_bang);
 
 addmess((method)stein_reset,"reset", A_GIMME, 0);
 addmess((method)stein_set,"set", A_GIMME, 0);
 addint((method)stein_int);
 addfloat((method)stein_float);
 addmess((method)stein_lambda,"lambda", A_DEFFLOAT, 0);
 addmess((method)stein_seed,"seed", A_DEFFLOAT, 0);
 addmess((method)stein_om,"om", A_DEFLONG, 0);
 
 addmess((method)stein_assist,"assist", A_CANT, 0);
 post("A-Chaos Lib :: a-stein  " __DATE__" "__TIME__"                                   ©   a n d r Ž s i e r   2 0 0 4   all rights reserved",tick, 0);
}


void stein_assist(stein *x, void *b, long m, long a, char *s)
{
    if (m==1) { sprintf(s,"bang, (int), (float)"); }   
    else if (m==2&&a==0) { sprintf(s,"(float) stein    e         c            h              o"); }
}



/*

ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ
a - verhulst © andrŽ sier 20030914
		
		distribuicao verhulsta

ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ
*/

#include "ext.h"
#include "ext_common.h"

//#include "math.h"




typedef struct	
{
	t_object 	        c_ob;			
	void 		        *c_out;		
	
	double lambda, seed, seedinit; 
	Boolean om;
} verhulst;


void *verhulst_new (Symbol *msg, short argc, Atom *argv);
void verhulst_bang (verhulst *x);
void verhulst_calc (verhulst *x);
void verhulst_set (verhulst *x, Symbol *msg, short argc, Atom *argv);  
void verhulst_reset(verhulst *x, Symbol *msg, short argc, Atom *argv);

void verhulst_echo (verhulst *x, float echo);
void verhulst_int(verhulst *x, int n);
void verhulst_float(verhulst *x, float n);

void verhulst_lambda (verhulst *x, float echo);
void verhulst_seed (verhulst *x, float echo);

void verhulst_assist(verhulst *x, void *b, long m, long a, char *s);
void *verhulst_class;


void *verhulst_new (Symbol *msg, short argc, Atom *argv) //input the args 
{
	 verhulst *x;
	 int i;
	 
	 x=(verhulst *)newobject(verhulst_class);

	 x->c_out=floatout(x);
	 
	x->seed = 0.5f;
	x->seedinit = 0.5f;
	x->lambda = 2.89f;
	x->om = 0;

	verhulst_set(x, msg, argc, argv);

	 return(x);	
}

void verhulst_set (verhulst *x, Symbol *msg, short argc, Atom *argv) //input the args 
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

void  verhulst_reset (verhulst *x, Symbol *msg, short argc, Atom *argv)
{	
	x->seed = x->seedinit;
}


void  verhulst_calc (verhulst *x)
{
	double lambda, seed ;
	
	lambda = x->lambda;
	seed = x->seed;
	
	x->seed = seed + (seed * lambda * (1.0-seed));

}



void verhulst_echo (verhulst *x, float echo)
{
 	x->lambda = echo;
	if (x->om) verhulst_bang(x);
}

void verhulst_int(verhulst *x, int n)     { verhulst_echo(x, n*0.01);}
void verhulst_float(verhulst *x, float n) { verhulst_echo(x, n);}


void verhulst_om(verhulst *x, int n)     { x->om = (n>0);}


void verhulst_bang (verhulst *x)
{

    outlet_float(x->c_out, x->seed); 
	
    verhulst_calc(x);   //next
    
}

void verhulst_lambda (verhulst *x, float echo)
{
	x->lambda = echo;
	if (x->om) verhulst_bang(x);
}
void verhulst_seed (verhulst *x, float echo)
{
	x->seed = echo;
	x->seedinit = echo;
	if (x->om) verhulst_bang(x);
}


void main(void)
{
 long int tick = gettime();
 setup((t_messlist**)&verhulst_class,(method)verhulst_new,0L,(short)sizeof(verhulst),0L,
 A_GIMME,0);

 addbang((method)verhulst_bang);
 
 addmess((method)verhulst_reset,"reset", A_GIMME, 0);
 addmess((method)verhulst_set,"set", A_GIMME, 0);
 addint((method)verhulst_int);
 addfloat((method)verhulst_float);
 addmess((method)verhulst_lambda,"lambda", A_DEFFLOAT, 0);
 addmess((method)verhulst_seed,"seed", A_DEFFLOAT, 0);
 addmess((method)verhulst_om,"om", A_DEFLONG, 0);
 
 addmess((method)verhulst_assist,"assist", A_CANT, 0);
 post("A-Chaos Lib :: a-verhulst  " __DATE__" "__TIME__"                                   ©   a n d r Ž s i e r   2 0 0 4   all rights reserved",tick, 0);
}


void verhulst_assist(verhulst *x, void *b, long m, long a, char *s)
{
    if (m==1) { sprintf(s,"bang, (int), (float)"); }   
    else if (m==2&&a==0) { sprintf(s,"(float) verhulst    e         c            h              o"); }
}



/*

ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ
a - logistic © andrŽ sier 20030914
		
		distribuicao logistica

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
} logistic;


void *logistic_new (Symbol *msg, short argc, Atom *argv);
void logistic_bang (logistic *x);
void logistic_calc (logistic *x);
void logistic_set (logistic *x, Symbol *msg, short argc, Atom *argv);  
void logistic_reset(logistic *x, Symbol *msg, short argc, Atom *argv);

void logistic_echo (logistic *x, float echo);
void logistic_int(logistic *x, int n);
void logistic_float(logistic *x, float n);

void logistic_lambda (logistic *x, float echo);
void logistic_seed (logistic *x, float echo);

void logistic_assist(logistic *x, void *b, long m, long a, char *s);
void *logistic_class;


void *logistic_new (Symbol *msg, short argc, Atom *argv) //input the args 
{
	 logistic *x;
	 int i;
	 
	 x=(logistic *)newobject(logistic_class);

	 x->c_out=floatout(x);
	 
	x->seed = 0.777f;
	x->seedinit = 0.777f;
	x->lambda = 3.9f;
	x->om = 0;

	logistic_set(x, msg, argc, argv);

	 return(x);	
}

void logistic_set (logistic *x, Symbol *msg, short argc, Atom *argv) //input the args 
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

void  logistic_reset (logistic *x, Symbol *msg, short argc, Atom *argv)
{	
	x->seed = x->seedinit;
}


void  logistic_calc (logistic *x)
{
	double lambda, seed ;
	
	lambda = x->lambda;
	seed = x->seed;
	
	x->seed = lambda * seed * (1.0-seed);

}



void logistic_echo (logistic *x, float echo)
{
 	x->lambda = echo;
	if (x->om) logistic_bang(x);
}

void logistic_int(logistic *x, int n)     { logistic_echo(x, n*0.01);}
void logistic_float(logistic *x, float n) { logistic_echo(x, n);}


void logistic_om(logistic *x, int n)     { x->om = (n>0);}


void logistic_bang (logistic *x)
{

    outlet_float(x->c_out, x->seed); 
	
    logistic_calc(x);   //next
    
}

void logistic_lambda (logistic *x, float echo)
{
	x->lambda = echo;
	if (x->om) logistic_bang(x);
}
void logistic_seed (logistic *x, float echo)
{
	x->seed = echo;
	x->seedinit = echo;
	if (x->om) logistic_bang(x);
}


void main(void)
{
 long int tick = gettime();
 setup((t_messlist**)&logistic_class,(method)logistic_new,0L,(short)sizeof(logistic),0L,
 A_GIMME,0);

 addbang((method)logistic_bang);
 
 addmess((method)logistic_reset,"reset", A_GIMME, 0);
 addmess((method)logistic_set,"set", A_GIMME, 0);
 addint((method)logistic_int);
 addfloat((method)logistic_float);
 addmess((method)logistic_lambda,"lambda", A_DEFFLOAT, 0);
 addmess((method)logistic_seed,"seed", A_DEFFLOAT, 0);
 addmess((method)logistic_om,"om", A_DEFLONG, 0);
 
 addmess((method)logistic_assist,"assist", A_CANT, 0);
 post("A-Chaos Lib :: a-logistic  " __DATE__" "__TIME__"                                   ©   a n d r Ž s i e r   2 0 0 4   all rights reserved",tick, 0);
}


void logistic_assist(logistic *x, void *b, long m, long a, char *s)
{
    if (m==1) { sprintf(s,"bang, (int), (float)"); }   
    else if (m==2&&a==0) { sprintf(s,"(float) l o j i s t i c    e         c            h              o"); }
}



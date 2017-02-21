/*

ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ
a - logistic1 © andrŽ sier 20030914
		
		distribuicao logistic1a

ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ
*/

#include "ext.h"
#include "ext_common.h"

//#include "math.h"




typedef struct	
{
	t_object 	        c_ob;			
	void 		        *c_out;		
	
	double lambda, gamma, seed, seedinit; 
	Boolean om;
} logistic1;


void *logistic1_new (Symbol *msg, short argc, Atom *argv);
void logistic1_bang (logistic1 *x);
void logistic1_calc (logistic1 *x);
void logistic1_set (logistic1 *x, Symbol *msg, short argc, Atom *argv);  
void logistic1_reset(logistic1 *x, Symbol *msg, short argc, Atom *argv);

void logistic1_echo (logistic1 *x, float echo);
void logistic1_int(logistic1 *x, int n);
void logistic1_float(logistic1 *x, float n);

void logistic1_lambda (logistic1 *x, float echo);
void logistic1_seed (logistic1 *x, float echo);

void logistic1_assist(logistic1 *x, void *b, long m, long a, char *s);
void *logistic1_class;


void *logistic1_new (Symbol *msg, short argc, Atom *argv) //input the args 
{
	 logistic1 *x;
	 int i;
	 
	 x=(logistic1 *)newobject(logistic1_class);

	 x->c_out=floatout(x);
	 
	x->seed = 0.777f;
	x->seedinit = 0.777f;
	x->lambda = 3.9f;
	x->gamma = 3.43f;
	x->om = 0;

	logistic1_set(x, msg, argc, argv);

	 return(x);	
}

void logistic1_set (logistic1 *x, Symbol *msg, short argc, Atom *argv) //input the args 
{
	
	if (argc) {

		if (argc > 2) {
			if (argv[2].a_type == A_FLOAT)
				x->gamma = argv[2].a_w.w_float;
			if (argv[2].a_type == A_LONG)
				x->gamma = (double)argv[2].a_w.w_long;			
		}
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

void  logistic1_reset (logistic1 *x, Symbol *msg, short argc, Atom *argv)
{	
	x->seed = x->seedinit;
}


void  logistic1_calc (logistic1 *x)
{
	double lambda, seed, gamma ;
	
	gamma = x->gamma;
	lambda = x->lambda;
	seed = x->seed;
	
	x->seed = (seed*lambda) -(gamma*seed*seed);

}



void logistic1_echo (logistic1 *x, float echo)
{
 	x->lambda = echo;
	if (x->om) logistic1_bang(x);
}

void logistic1_int(logistic1 *x, int n)     { logistic1_echo(x, n*0.01);}
void logistic1_float(logistic1 *x, float n) { logistic1_echo(x, n);}


void logistic1_om(logistic1 *x, int n)     { x->om = (n>0);}


void logistic1_bang (logistic1 *x)
{

    outlet_float(x->c_out, x->seed); 
	
    logistic1_calc(x);   //next
    
}

void logistic1_lambda (logistic1 *x, float echo)
{
	x->lambda = echo;
	if (x->om) logistic1_bang(x);
}
void logistic1_gamma (logistic1 *x, float echo)
{
	x->gamma = echo;
	if (x->om) logistic1_bang(x);
}
void logistic1_seed (logistic1 *x, float echo)
{
	x->seed = echo;
	x->seedinit = echo;
	if (x->om) logistic1_bang(x);
}


void main(void)
{
 long int tick = gettime();
 setup((t_messlist**)&logistic1_class,(method)logistic1_new,0L,(short)sizeof(logistic1),0L,
 A_GIMME,0);

 addbang((method)logistic1_bang);
 
 addmess((method)logistic1_reset,"reset", A_GIMME, 0);
 addmess((method)logistic1_set,"set", A_GIMME, 0);
 addint((method)logistic1_int);
 addfloat((method)logistic1_float);
 addmess((method)logistic1_lambda,"lambda", A_DEFFLOAT, 0);
 addmess((method)logistic1_gamma,"gamma", A_DEFFLOAT, 0);
 addmess((method)logistic1_seed,"seed", A_DEFFLOAT, 0);
 addmess((method)logistic1_om,"om", A_DEFLONG, 0);
 
 addmess((method)logistic1_assist,"assist", A_CANT, 0);
 post("A-Chaos Lib :: a-logistic1  " __DATE__" "__TIME__"                                   ©   a n d r Ž s i e r   2 0 0 4   all rights reserved",tick, 0);
}


void logistic1_assist(logistic1 *x, void *b, long m, long a, char *s)
{
    if (m==1) { sprintf(s,"bang, (int), (float)"); }   
    else if (m==2&&a==0) { sprintf(s,"(float) l o j i s t i c    e         c            h              o"); }
}




/*

ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ
a - torus © andrŽ sier 20030914
		
		distribuicao torusa

ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ
*/

#include "ext.h"
#include "ext_common.h"

#include "math.h"

#define	pi  		3.14159265358979323846264338327950288419716939937510f
#define twopi			2*pi


typedef struct	
{
	t_object 	        c_ob;			
	void 		        *c_out;		
	void 		        *c_out2;		
	
	double x0, y0, cr; 
	double x0init, y0init, crinit; 
	
	Boolean om;
} torus;


void *torus_new (Symbol *msg, short argc, Atom *argv);
void torus_bang (torus *x);
void torus_calc (torus *x);
void torus_set (torus *x, Symbol *msg, short argc, Atom *argv);  
void torus_reset(torus *x, Symbol *msg, short argc, Atom *argv);


void torus_x0 (torus *x, float echo);
void torus_y0 (torus *x, float echo);
void torus_cr (torus *x, float echo);

void torus_assist(torus *x, void *b, long m, long a, char *s);
void *torus_class;



void *torus_new (Symbol *msg, short argc, Atom *argv) 
{
	 torus *x;
	 int i;
	 
	 x=(torus *)newobject(torus_class);

	 x->c_out2=floatout(x);
	 x->c_out=floatout(x);
	 
	x->x0 = 0.777f;
	x->y0 = 1.2f;
	x->cr = 1.26f;
	x->x0init = 0.777f;
	x->y0init = 1.2f;
	x->crinit = 1.26f;

	x->om = 0;

	torus_set(x, msg, argc, argv);

	 return(x);	
}

void torus_set (torus *x, Symbol *msg, short argc, Atom *argv) //input the args 
{
	
	if (argc) {

		if (argc > 2) {
			if (argv[2].a_type == A_FLOAT)
				x->crinit = argv[2].a_w.w_float;
			if (argv[2].a_type == A_LONG)
				x->crinit = (double)argv[2].a_w.w_long;			
			x->cr = x->crinit;
		}
		if (argc > 1) {
			if (argv[1].a_type == A_FLOAT)
				x->y0init = argv[1].a_w.w_float;
			if (argv[1].a_type == A_LONG)
				x->y0init = (double)argv[1].a_w.w_long;			
			x->y0 = x->y0init;
		}
		if (argc > 0) {
			if (argv[0].a_type == A_FLOAT)
				x->x0init = argv[0].a_w.w_float;
			if (argv[0].a_type == A_LONG)
				x->x0init = (double)argv[0].a_w.w_long;
				
			x->x0 = x->x0init;
		}

	} //end if args
}

void  torus_reset (torus *x, Symbol *msg, short argc, Atom *argv)
{	
	x->cr = x->crinit;
	x->x0 = x->x0init;
	x->y0 = x->y0init;
}


void  torus_calc (torus *x)
{
	double x0, y0, cr;
	
	x0 = x->x0;
	y0 = x->y0;
	cr = x->cr;
	
	x->x0 = fmod((x0+cr*sin(y0)), twopi);
	x->y0 = fmod((x0+y0), twopi);

}



void torus_om(torus *x, int n)     { x->om = (n>0);}


void torus_bang (torus *x)
{

    outlet_float(x->c_out2, x->y0); 
    outlet_float(x->c_out, x->x0); 
	
    torus_calc(x);   //next
    
}

void torus_cr (torus *x, float echo)
{
	x->cr = echo;
	if (x->om) torus_bang(x);
}
void torus_x0 (torus *x, float echo)
{
	x->x0 = echo;
	if (x->om) torus_bang(x);
}
void torus_y0 (torus *x, float echo)
{
	x->y0 = echo;
	if (x->om) torus_bang(x);
}


void main(void)
{
 long int tick = gettime();
 setup((t_messlist**)&torus_class,(method)torus_new,0L,(short)sizeof(torus),0L,
 A_GIMME,0);

 addbang((method)torus_bang);
 
 addmess((method)torus_reset,"reset", A_GIMME, 0);
 addmess((method)torus_set,"set", A_GIMME, 0);
 addmess((method)torus_cr,"cr", A_DEFFLOAT, 0);
 addmess((method)torus_x0,"x", A_DEFFLOAT, 0);
 addmess((method)torus_y0,"y", A_DEFFLOAT, 0);
 addmess((method)torus_om,"om", A_DEFLONG, 0);
 
 addmess((method)torus_assist,"assist", A_CANT, 0);
 post("A-Chaos Lib :: a-torus  " __DATE__" "__TIME__"                                   ©   a n d r Ž s i e r   2 0 0 4   all rights reserved",tick, 0);
}


void torus_assist(torus *x, void *b, long m, long a, char *s)
{
    if (m==1) { sprintf(s,"bang, (int), (float)"); }   
    else if (m==2&&a==0) { sprintf(s,"(float) x torus    e         c            h              o"); }
    else if (m==2&&a==1) { sprintf(s,"(float) y torus    e         c            h              o"); }
}



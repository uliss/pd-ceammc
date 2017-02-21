/*
*/

#include "ext.h"
#include "ext_common.h"

//#include "math.h"

#define rosx(y,z)		-((y)+(z))
#define rosy(x,y,a)		(x)+((a)*(y))
#define rosz(x,z,b,c)	(b)+((x)*(z))-((c)*(z))



typedef struct	
{
	t_object 	        c_ob;			
	void 		        *c_out;		// value
	void 		        *c_out2;		// value
	void 		        *c_out3;		// value
	
	double		a, b, c, nx, ny, nz,dt; //eq variables
	double		ainit, binit, cinit, nxinit, nyinit, nzinit,dtinit; //init eq variables

	Boolean om;
} rossler;


void *rossler_new (Symbol *msg, short argc, Atom *argv);
void rossler_bang (rossler *x);

//user variables
void rossler_a (rossler *x, double echo);
void rossler_b (rossler *x, double echo);
void rossler_c (rossler *x, double echo);
void rossler_nx (rossler *x, double echo);
void rossler_ny (rossler *x, double echo);
void rossler_nz (rossler *x, double echo);
void rossler_dt (rossler *x, double echo);

//calc
void rossler_calc (rossler *x);
void rossler_reset (rossler *x, Symbol *msg, short argc, Atom *argv); 
void rossler_set (rossler *x, Symbol *msg, short argc, Atom *argv) ; 

void rossler_assist(rossler *x, void *b, long m, long a, char *s);
void *rossler_class;


void *rossler_new (Symbol *msg, short argc, Atom *argv) //input the args 
{
	 rossler *x;
	 int i;
	 
	 x=(rossler *)newobject(rossler_class);

	 x->c_out3=floatout(x);
	 x->c_out2=floatout(x);
	 x->c_out=floatout(x);

	 //classic rossler
		 x->a=0.2;
		 x->b=0.2; 
		 x->c=5.7; 
		 x->nx=1; 
		 x->ny=1; 
		 x->nz=1;
		 x->dt=0.01;

		 x->ainit=0.2;
		 x->binit=0.2; 
		 x->cinit= 5.7; 
		 x->nxinit=1; 
		 x->nyinit=1; 
		 x->nzinit=1;
		 x->dtinit=0.01;
		 
		 x->om = 0;

	rossler_set(x, msg, argc, argv);

	 return(x);	
}

void rossler_om(rossler *x, int n)     { x->om = (n>0);}


void rossler_set (rossler *x, Symbol *msg, short ac, Atom *av) //input the args 
{
	if (ac) {

		if (ac > 6) {
			if (av[6].a_type == A_LONG)
				x->dtinit = (double)av[6].a_w.w_long;
			else if (av[6].a_type == A_FLOAT)
				x->dtinit = av[6].a_w.w_float;
			x->dt = x->dtinit;
		}
		if (ac > 5) {
			if (av[5].a_type == A_LONG)
				x->cinit = (double)av[5].a_w.w_long;
			else if (av[5].a_type == A_FLOAT)
				x->cinit = av[5].a_w.w_float;
			x->c = x->cinit;
		}
		if (ac > 4) {
			if (av[4].a_type == A_LONG)
				x->binit = (double)av[4].a_w.w_long;
			else if (av[4].a_type == A_FLOAT)
				x->binit = av[4].a_w.w_float;
			x->b = x->binit;
		}
		if (ac > 3) {
			if (av[3].a_type == A_LONG)
				x->ainit = (double)av[3].a_w.w_long;
			else if (av[3].a_type == A_FLOAT)
				x->ainit = av[3].a_w.w_float;
			x->a = x->ainit;
		}
		if (ac > 2) {
			if (av[2].a_type == A_LONG)
				x->nzinit = (double)av[2].a_w.w_long;
			else if (av[2].a_type == A_FLOAT)
				x->nzinit = av[2].a_w.w_float;
			x->nz = x->nzinit;
		}
		if (ac > 1) {
			if (av[1].a_type == A_LONG)
				x->nyinit = (double)av[1].a_w.w_long;
			else if (av[1].a_type == A_FLOAT)
				x->nyinit = av[1].a_w.w_float;
			x->ny = x->nyinit;
		}	
		if (ac > 0) {
			if (av[0].a_type == A_LONG)
				x->nxinit = (double)av[0].a_w.w_long;
			else if (av[0].a_type == A_FLOAT)
				x->nxinit = av[0].a_w.w_float;
			x->nx = x->nxinit;
		}


	
	} // end if args
}

void rossler_reset (rossler *x, Symbol *msg, short argc, Atom *argv)
{
	x->nx = x->nxinit;
	x->ny = x->nyinit;
	x->nz = x->nzinit;
	x->a = x->ainit;
	x->b = x->binit;
	x->c = x->cinit;
	x->dt = x->dtinit;

}



void  rossler_calc (rossler *x)
{
	double	a, b, c, dt, x0, y0, z0;
	double  fx, fy, fz, ffx, ffy, ffz, dt2;
	double	dtfx, dtfy, dtfz;
	
	//local copies
	a = x->a;
	b = x->b;
	c = x->c;
	dt = x->dt;
	dt2 = dt/2.;
	x0 = x->nx;
	y0 = x->ny;
	z0 = x->nz;


	fx = rosx(y0,z0);
	fy = rosy(x0,y0,a);
	fz = rosz(x0,z0,b,c);

	ffx = rosx((y0+dt*fy), (z0+(dt*fz)));
	ffy = rosy((x0+dt*x0), (y0+dt*y0), a);
	ffz = rosz((x0+dt*fx), (z0+dt*z0), b, c);

	x->nx += (dt2*(fx+ffx));
	x->ny += (dt2*(fy+ffy));
	x->nz += (dt2*(fz+ffz));
}




void rossler_a (rossler *x, double echo)
{
	x->a = (double)echo;
//	x->ainit = (double)echo;
	if (x->om) rossler_bang(x);
}


void rossler_b (rossler *x, double echo)
{
	x->b = (double)echo;
//	x->rinit = (double)echo;
	if (x->om) rossler_bang(x);
}

void rossler_c (rossler *x, double echo)
{
	x->c = (double)echo;
//	x->cinit = (double)echo;
	if (x->om) rossler_bang(x);
}

void rossler_nx (rossler *x, double echo)
{
	x->nx = (double)echo;
//	x->nxinit = (double)echo;
	if (x->om) rossler_bang(x);
}

void rossler_ny (rossler *x, double echo)
{
	x->ny = (double)echo;
//	x->nyinit = (double)echo;
	if (x->om) rossler_bang(x);
}

void rossler_nz (rossler *x, double echo)
{
	x->nz = (double)echo;
//	x->nzinit = (double)echo;
	if (x->om) rossler_bang(x);
}

void rossler_dt (rossler *x, double echo)
{
	x->dt = (double)echo;
//	x->dtinit = (double)echo;
	if (x->om) rossler_bang(x);
}


void rossler_bang (rossler *x)
{

	outlet_float(x->c_out3, x->nz); 
	outlet_float(x->c_out2, x->ny); 
	outlet_float(x->c_out, x->nx); 

    rossler_calc(x);   //next
    
}



void main(void)
{
 long int tick = gettime();
 setup((t_messlist**)&rossler_class,(method)rossler_new,0L,(short)sizeof(rossler),0L,
 A_GIMME,0);

 addbang((method)rossler_bang);
 addmess((method)rossler_set,"set", A_GIMME, 0);
 addmess((method)rossler_reset,"reset", A_GIMME, 0);

 addmess((method)rossler_dt,"dt", A_DEFFLOAT, 0);
 addmess((method)rossler_a,"a", A_DEFFLOAT, 0);
 addmess((method)rossler_b,"b", A_DEFFLOAT, 0);
 addmess((method)rossler_c,"c", A_DEFFLOAT, 0);
 addmess((method)rossler_nx,"x", A_DEFFLOAT, 0);
 addmess((method)rossler_ny,"y", A_DEFFLOAT, 0);
 addmess((method)rossler_nz,"z", A_DEFFLOAT, 0);
 addmess((method)rossler_om,"om", A_DEFLONG, 0);

 addmess((method)rossler_assist,"assist", A_CANT, 0);
 post("A-Chaos Lib :: a-rossler  " __DATE__" "__TIME__"                                   ©   a n d r Ž s i e r   2 0 0 4   all rights reserved",tick, 0);
}


void rossler_assist(rossler *x, void *b, long m, long a, char *s)
{
    if (m==1) { sprintf(s,"bang, (int), (float), ·, ¶ª"); }   
    else if (m==2&&a==0) { sprintf(s,"(float) x  rossler   e         c            h              o"); }
    else if (m==2&&a==1) { sprintf(s,"(float) y  rossler   e         c            h              o"); }
    else if (m==2&&a==2) { sprintf(s,"(float) z  rossler   e         c            h              o"); }
//    else if (m==2&&a==1) { sprintf(s,"(lisy)       e             c                h                   o"); }
}



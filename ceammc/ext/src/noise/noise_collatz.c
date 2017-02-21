/*

ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ
collatz - © Copyright sier 2002
		
		collatz conjecture output
ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ
*/

#include "ext.h"

typedef struct	
{
	t_object 	        c_ob;			
	void 		        *c_out;			
	long 		        c_value;			
	long 		        c_offset;
	Boolean		c_mode,om;			
} atz;


void *atz_new (long inputarg, long offset);
void atz_offset (atz *x, long offset);
void atz_bang (atz *x);
void atz_int (atz *x, long number);
void atz_mode (atz *x, long number);
void atz_om (atz *x, long number);
int   atz_calc (atz *x, int number);
void atz_assist(atz *x, void *b, long m, long a, char *s);

void *atz_class;


void atz_mode (atz *x, long number){ x->c_mode = number;if(atz_om) atz_bang(x); }

void *atz_new (long inputarg, long offset) 
{
 atz *x;
 x=(atz *)newobject(atz_class);
 x->c_out=intout(x);

x->c_value=x->c_offset=0; //init
x->c_mode=x->om =0;
 x->c_value=inputarg; if(!inputarg)x->c_value=71; //seed it
 x->c_offset=offset; if(!offset)x->c_offset=0;
 return(x);	///the obj, nice and clean, ready to use
}


int  atz_calc (atz *x, int number)
{
	 short int stub;
	 stub = number % 2; 			//par ou impar
	 
	if(!x->c_mode) { 				// L.Collatz collatz mode
		 if (!stub) number /= 2;		//par ]e metade
		 else number = 3*number+1;	//impar ]e 3n+1
		 return (number);
		}
	else if (x->c_mode) { 				// Terras collatz mode
		 if (!stub) number /= 2;		//par ]e metade
		 else number = 0.5*(3*number+1);	//impar ]e 1/2(3n+1)
		 return (number);
		}
}

void atz_offset (atz *x, long offset){ x->c_offset=offset; if(atz_om) atz_bang(x);}
void atz_om (atz *x, long number) { x->om = (number>0);}

void atz_int (atz *x, long number) 
{
    x->c_value=atz_calc(x, number-1); //guardar dentro do objecto calculado
	if(atz_om) atz_bang(x);
}

void atz_bang (atz *x)
{

    outlet_int(x->c_out, x->c_value + x->c_offset);

    x->c_value=atz_calc(x, x->c_value); //run again
}



void main(void)
{
int tick = gettime();
setup((t_messlist**)&atz_class,(method)atz_new,0L,(short)sizeof(atz),0L, A_DEFLONG, A_DEFLONG, 0);
 addbang((method)atz_bang);
 addint((method)atz_int);
 addmess((method)atz_offset,"offset", A_DEFLONG, 0);
 addmess((method)atz_mode,"mode", A_DEFLONG, 0);
 addmess((method)atz_om,"om", A_DEFLONG, 0);
 addmess((method)atz_assist,"assist", A_CANT, 0);
 post("A-Chaos Lib :: a-collatz  " __DATE__" "__TIME__"                                   ©   a n d r Ž s i e r   2 0 0 4   all rights reserved",tick, 0);
}

void atz_assist(atz *x, void *b, long m, long a, char *s)
{
    if (m==1) { sprintf(s,"bang, (int), offset"); }   
    else if (m==2) { sprintf(s,"(int)"); }
}